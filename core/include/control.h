#pragma once

#include <stdint.h>

union ControlState {
	uint8_t data[3];
	struct {
		unsigned lcd_enabled:1;
		unsigned cart_enabled:1;
	};
};

uint8_t ctrl_read_reg(MachineState& cpu, uint32_t address);
void ctrl_write_reg(MachineState& cpu, uint8_t data, uint32_t address);
