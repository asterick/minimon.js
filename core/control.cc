#include <stdint.h>

#include "control.h"

void ControlState::reset() {
	this->data[0] = 0;
	this->data[1] = 0;
	this->data[2] = 0;
}

uint8_t ControlState::read(uint32_t address) {
	return this->data[address & 3];
}

void ControlState::write(uint8_t data, uint32_t address) {
	this->data[address & 3] = data;

	if (address == 0x2000) {
		this->lcd_enabled = (data & 1) != 0;
		this->cart_enabled = (data & 2) != 0;
	}
}
