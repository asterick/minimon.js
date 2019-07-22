#include <stdint.h>

#include "machine.h"

void ctrl_reset(MachineState& cpu) {
}

uint8_t ctrl_read_reg(MachineState& cpu, uint32_t address) {
	return cpu.ctrl.data[address & 3];
}

void ctrl_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	cpu.ctrl.data[address & 3] = data;
}
