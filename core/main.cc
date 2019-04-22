#include "system.h"

static ProcessorState minimon;
static uint8_t memory[0x20000];

__attribute__ ((visibility ("default"))) extern "C"
ProcessorState* const get_machine() {
	return &minimon;
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t* const get_bios() {
	return memory;
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t cpu_read8(ProcessorState& cpu, uint32_t address) {
	if (address < 0x2000 || address >= 0x2100) {
		// NOTE: This does not properly handle wrap around
		return memory[address];
	} else {
		return 0;
	}
}

__attribute__ ((visibility ("default"))) extern "C"
void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address) {
	if (address >= 0x1000 && address < 0x2000) {
		memory[address] = data;
	}
}
