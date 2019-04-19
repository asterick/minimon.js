#include "system.h"

static ProcessorState minimon;
static uint8_t bios[0x1000];

__attribute__ ((visibility ("default"))) extern "C"
ProcessorState* const get_machine() {
	return &minimon;
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t* const get_bios() {
	return bios;
}

__attribute__ ((visibility ("default"))) extern "C"
uint8_t cpu_read8(ProcessorState& cpu, uint32_t address) {
	if (address < 0x1000) {
		return bios[address];
	} else {
		return 0;
	}
}

__attribute__ ((visibility ("default"))) extern "C"
void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address) {
}
