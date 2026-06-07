// Headless regression harness for the minimon core.
//
// Runs the machine for N virtual seconds and prints a hash of semantic
// state (register values, RAM, LCD gddram) each second. The hash is
// layout-independent on purpose: it is used to prove that refactors are
// bit-identical in behavior, surviving struct layout changes.
//
// With no ROM (or "-"), the machine boots the embedded BIOS with no
// cartridge inserted — fully deterministic and self-contained, which is
// what CI checks against baseline-bios.txt. Point it at a .min ROM for
// local verification against game behavior.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "machine.h"

static Machine::State machine;

extern "C" void debug_print(const void* data) {
	// Uncomment to see core debug spew:
	// fprintf(stderr, "[core] %s\n", (const char*)data);
}

extern "C" void flip_screen(void* lcd) {}
extern "C" void audio_push(float*) {}

static uint64_t fnv1a(const void* data, size_t len, uint64_t h) {
	const uint8_t* p = (const uint8_t*)data;
	while (len--) {
		h ^= *p++;
		h *= 1099511628211ull;
	}
	return h;
}

static uint64_t hash_state(Machine::State& m) {
	// Hash register *values* (not struct memory) so this survives layout changes
	uint16_t regs[] = {
		m.reg.sc, m.reg.cc, m.reg.ba(), m.reg.hl(),
		m.reg.pc, m.reg.sp, m.reg.ix, m.reg.iy,
		m.reg.br, m.reg.ep, m.reg.xp, m.reg.yp,
		m.reg.cb, m.reg.nb, (uint16_t)m.status
	};

	uint64_t h = 1469598103934665603ull;
	for (size_t i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
		uint8_t pair[2] = { (uint8_t)regs[i], (uint8_t)(regs[i] >> 8) };
		h = fnv1a(pair, 2, h);
	}

	h = fnv1a(m.ram, sizeof(m.ram), h);
	h = fnv1a(m.lcd.gddram, sizeof(m.lcd.gddram), h);

	return h;
}

// The BIOS idle loop alone leaves the cartridge-driven hardware cold, so
// this scripted workout drives it through the bus instead: patterned
// tiles and OAM in RAM, the blitter rendering map + sprites (with copy
// to the LCD, whose gddram is hashed), all three timers counting in
// both widths with compare IRQs, and audio enabled off timer 2.
static void exercise(Machine::State& m) {
	// Tile pattern + map indices + sprite tiles into RAM (map_base /
	// sprite_base point at main RAM, 0x1000-)
	for (int i = 0; i < 0x200; i++) cpu_write(m, (uint8_t)(i * 7 + 3), 0x1800 + i);
	for (int i = 0; i < 0x60; i++)  cpu_write(m, (uint8_t)(i & 15), 0x1300 + i);

	// OAM: a few sprites with flips/inverts across the screen edges
	static const uint8_t oam[] = {
		8, 8, 1, 0b1000,        // plain
		20, 4, 2, 0b1001,       // x-flip
		4, 30, 3, 0b1010,       // y-flip
		90, 60, 1, 0b1100,      // invert, partially offscreen
		120, 100, 2, 0b1000,    // wrapped coordinates
	};
	for (size_t i = 0; i < sizeof(oam); i++) cpu_write(m, oam[i], 0x1300 + 0x60 + i);

	// Blitter: map at 0x1800, sprites at 0x1800, map+sprites+copy, 16x12 map
	cpu_write(m, 0x18, 0x2083); cpu_write(m, 0x00, 0x2082); cpu_write(m, 0x00, 0x2084);
	cpu_write(m, 0x18, 0x2088); cpu_write(m, 0x00, 0x2087); cpu_write(m, 0x00, 0x2089);
	cpu_write(m, 5, 0x2085); cpu_write(m, 3, 0x2086);       // scroll
	cpu_write(m, 0b011110, 0x2080);                         // enables + 16x12
	cpu_write(m, 0b0010, 0x2081);                           // frame divider

	// Timers: t0 16-bit, t1/t2 split 8-bit, mixed prescales, t2 compare
	// feeds the audio square wave
	cpu_write(m, 0b00110011, 0x2019);                       // osc enables + sources
	cpu_write(m, 0x3F, 0x2018); cpu_write(m, 0x52, 0x201A); cpu_write(m, 0x25, 0x201C);
	cpu_write(m, 0xE8, 0x2032); cpu_write(m, 0x03, 0x2033); // t0 preset 0x03E8
	cpu_write(m, 0x40, 0x203A); cpu_write(m, 0x80, 0x203B); // t1 presets
	cpu_write(m, 0x60, 0x204A); cpu_write(m, 0x30, 0x204B); // t2 presets
	cpu_write(m, 0x20, 0x204C); cpu_write(m, 0x00, 0x204D); // t2 compare
	cpu_write(m, 0b10000111, 0x2030);                       // t0: 16-bit, preset, run
	cpu_write(m, 0b00000111, 0x2038); cpu_write(m, 0b00000111, 0x2039);
	cpu_write(m, 0b00000111, 0x2048); cpu_write(m, 0b00000111, 0x2049);

	// Audio: enabled, mid volume
	cpu_write(m, 0b000, 0x2070); cpu_write(m, 0b010, 0x2071);

	// RTC: running (value ticks once per 0x8000 osc1 cycles)
	cpu_write(m, 0b01, 0x2008);
}

int main(int argc, char** argv) {
	const char* rom = (argc > 1) ? argv[1] : "-";
	int seconds = (argc > 2) ? atoi(argv[2]) : 10;
	bool has_cart = strcmp(rom, "-") != 0;
	bool do_exercise = strcmp(rom, "exercise") == 0;

	memset(&machine, 0, sizeof(machine));

	if (do_exercise) has_cart = false;

	if (has_cart) {
		FILE* f = fopen(rom, "rb");
		if (!f) {
			fprintf(stderr, "cannot open %s\n", rom);
			return 1;
		}

		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		fseek(f, 0, SEEK_SET);

		uint8_t* data = (uint8_t*)malloc(size);
		if (fread(data, 1, size, f) != (size_t)size) {
			fprintf(stderr, "short read\n");
			return 1;
		}
		fclose(f);

		// Mirror libretro's retro_load_game: headerless ROMs map at 0x2100
		long offset = (size >= 2 && data[0] == 'M' && data[1] == 'N') ? 0x2100 : 0;
		for (long i = 0; i < size; i++) {
			machine.cartridge[(i + offset) % sizeof(machine.cartridge)] = data[i];
		}
		free(data);
	}

	set_sample_rate(machine, 44100);
	cpu_reset(machine);
	// Buttons released (active low); cartridge-present is bit 9 low.
	// Must come after cpu_reset — Input::reset reverts to "no cartridge".
	update_inputs(machine, has_cart ? 0b0111111111 : 0b1111111111);

	if (do_exercise) {
		cpu_advance(machine, CPU_SPEED);	// let the BIOS settle first
		exercise(machine);
	}

	for (int s = 1; s <= seconds; s++) {
		if (do_exercise) {
			// Wiggle the pad each second to fire input edge IRQs
			update_inputs(machine, 0b1111111111 ^ ((s % 7) + 1));
		}

		cpu_advance(machine, CPU_SPEED);
		printf("%3d %016llx pc=%02x:%04x status=%d ctrl=%02x rtc=%06x+%04x\n", s,
			(unsigned long long)hash_state(machine),
			machine.reg.cb, machine.reg.pc, machine.status, machine.ctrl.data[0],
			machine.rtc.value & 0xFFFFFF, machine.rtc.prescale & 0xFFFF);
	}

	return 0;
}
