#include "machine.h"

static ProcessorState state;

static const uint8_t bios[0x2000] = {
	#include "bios.h"
};

__attribute__ ((visibility ("default"))) extern "C"
ProcessorState* const get_machine() {
	return &state;
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t cpu_read8(ProcessorState& cpu, uint32_t address) {
	if (address < 0x1000) {
		return bios[address];
	}  if (address < 0x2000) {
		return cpu.ram[address & 0xFFF];
	} else {
		return cpu.cartridge[address & 0x1FFFFF];
	}
}

__attribute__ ((visibility ("default"))) extern "C"
void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address) {
	if (address >= 0x1000 && address < 0x2000) {
		cpu.ram[address & 0xFFF] = data;
	}
}
