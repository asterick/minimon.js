#pragma once

#include <stdint.h>

union ControlState {
	uint8_t data[3];
	struct {
		unsigned lcd_enabled:1;
		unsigned cart_enabled:1;
	};
};

void ctrl_reset(ControlState& ctrl);
uint8_t ctrl_read_reg(ControlState& ctrl, uint32_t address);
void ctrl_write_reg(ControlState& ctrl, uint8_t data, uint32_t address);
