#include "debug.h"
#include "machine.h"

#include "lcd.h"
#include "blitter.h"
#include "irq.h"

const auto OSC1_SPEED	= 4000000;
const auto OSC3_SPEED	= 32768;
const auto MS_SPEED		= 1000;
const auto CPU_DIVIDER	= 4;

static const uint8_t bios[0x2000] = {
	#include "bios.h"
};

__attribute__ ((visibility ("default"))) extern "C"
ProcessorState* const get_machine() {
	static ProcessorState state;
	return &state;
}

__attribute__ ((visibility ("default"))) extern "C"
bool cpu_advance(ProcessorState& cpu, int ticks) {
	cpu.clocks += OSC1_SPEED / CPU_DIVIDER / MS_SPEED * ticks;

	while (cpu.clocks > 0) {
		cpu_step(cpu);
	}

	return true;
}

uint8_t cpu_read_reg(ProcessorState& cpu, uint32_t address) {
	dprintf(0x100, "Register read %x", address);
	return 0xCD;
}

void cpu_write_reg(ProcessorState& cpu, uint8_t data, uint32_t address) {
	dprintf(0x100, "Register write %x: %x", address, data);
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t cpu_read8(ProcessorState& cpu, uint32_t address) {
	if (address < 0x1000) {
		return bios[address];
	} else if (address < 0x2000) {
		return cpu.ram[address & 0xFFF];
	} else if (address < 0x2100) {
		return cpu_read_reg(cpu, address);
	} else {
		return cpu_read_cart(cpu, address);
	}
}

__attribute__ ((visibility ("default"))) extern "C"
void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address) {
	if (address >= 0x2100) {
		cpu_write_cart(cpu, data, address);
	} else if (address > 0x2000) {
		cpu_write_reg(cpu, data, address);
	} else if (address > 0x1000) {
		cpu.ram[address & 0xFFF] = data;
	}
}
