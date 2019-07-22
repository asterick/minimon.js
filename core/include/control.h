#pragma once

#include <stdint.h>

union ControlState {
	uint8_t data[3];

	bool lcd_enabled;
	bool cart_enabled;
};

void ctrl_reset(ControlState& ctrl);
uint8_t ctrl_read_reg(ControlState& ctrl, uint32_t address);
void ctrl_write_reg(ControlState& ctrl, uint8_t data, uint32_t address);
