#include "machine.h"

union FrameBuffer {
	uint8_t  bytes[96][8];
	uint64_t column[96];
};

static const uint8_t BIT_MASK[] = {
	0b00111111,
	0b00001111,
	0b11111000,
	0b11111111,
	0b00011111,
	0b01111111,
	0b01111111,
	0b11000000,
	0b11111111,
	0b00011111,
};

static void render(Machine::State& cpu) {
	FrameBuffer target;

	// Prepare background layer
	if (cpu.blitter.enable_map)	{
		// TODO: DRAW MAP HERE

		if (cpu.blitter.invert_map) {
			for (int x = 0; x < 96; x++) target.column[x] = ~target.column[x];
		}
	} else {
		for (int x = 0; x < 96; x++) {
			for (int y = 0; y < 8; y++) {
				target.bytes[x][y] = cpu.overlay.framebuffer[y][x];
			}
		}
	}

	if (cpu.blitter.enable_sprites) {
		// TODO: DRAW ALL SPRITES		
	}

	// Copy back to ram
	for (int x = 0; x < 96; x++) {
		for (int y = 0; y < 8; y++) {
			cpu.overlay.framebuffer[y][x] = target.bytes[x][y];
		}
	}
}

void Blitter::reset(Machine::State& cpu) {

}

void Blitter::clock(Machine::State& cpu, int osc3) {

}

uint8_t Blitter::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2080:
			return cpu.blitter.enables;
		case 0x2081:
			return 0 ;
		case 0x2082:
			return cpu.blitter.map_bytes[0];
		case 0x2083:
			return cpu.blitter.map_bytes[1];
		case 0x2084:
			return cpu.blitter.map_bytes[2];
		case 0x2085:
			return cpu.blitter.scroll_y;
		case 0x2086:
			return cpu.blitter.scroll_x;
		case 0x2087:
			return cpu.blitter.sprite_bytes[0];
		case 0x2088:
			return cpu.blitter.sprite_bytes[1];
		case 0x2089:
			return cpu.blitter.sprite_bytes[2];
		case 0x208A:
			return 0 ;
	}

	return 0;
}

void Blitter::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	data &= BIT_MASK[address & 0xF];

	switch (address) {
		case 0x2080:
			cpu.blitter.enables = data;
			break ;
		case 0x2081:
			break ;
		case 0x2082:
			cpu.blitter.map_bytes[0] = data;
			break ;
		case 0x2083:
			cpu.blitter.map_bytes[1] = data;
			break ;
		case 0x2084:
			cpu.blitter.map_bytes[2] = data;
			break ;
		case 0x2085:
			cpu.blitter.scroll_y = data;
			break ;
		case 0x2086:
			cpu.blitter.scroll_x = data;
			break ;
		case 0x2087:
			cpu.blitter.sprite_bytes[0] = data;
			break ;
		case 0x2088:
			cpu.blitter.sprite_bytes[1] = data;
			break ;
		case 0x2089:
			cpu.blitter.sprite_bytes[2] = data;
			break ;
	}
}
