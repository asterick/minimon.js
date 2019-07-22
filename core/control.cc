#include <stdint.h>

#include "control.h"

void ctrl_reset(ControlState& ctrl) {
	ctrl.data[0] = 0;
	ctrl.data[1] = 0;
	ctrl.data[2] = 0;
}

uint8_t ctrl_read_reg(ControlState& ctrl, uint32_t address) {
	return ctrl.data[address & 3];
}

void ctrl_write_reg(ControlState& ctrl, uint8_t data, uint32_t address) {
	ctrl.data[address & 3] = data;

	if (address == 0x2000) {
		ctrl.lcd_enabled = (data & 1) != 0;
		ctrl.cart_enabled = (data & 2) != 0;
	}
}
