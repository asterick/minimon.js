#include "system.h"

static ProcessorState state;

static const uint8_t bios[0x2000] = {
	#include "bios.h"
};

static uint8_t ram[0x1000];
static uint8_t memory[0x20000];

__attribute__ ((visibility ("default"))) extern "C"
ProcessorState* const get_machine() {
	return &state;
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t cpu_read8(ProcessorState& cpu, uint32_t address) {
	if (address < 0x1000) {
		return bios[address];
	}  if (address < 0x2000) {
		return ram[address & 0xFFF];
	} else {
		return memory[address & 0x1FFFFF];
	}
}

__attribute__ ((visibility ("default"))) extern "C"
void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address) {
	if (address >= 0x1000 && address < 0x2000) {
		ram[address & 0xFFF] = data;
	}
}
