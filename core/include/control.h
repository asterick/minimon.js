#pragma once

#include <stdint.h>

class ControlState {
public:
	bool lcd_enabled;
	bool cart_enabled;

	void reset();
	uint8_t read(uint32_t address);
	void write(uint8_t data, uint32_t address);

private:
	uint8_t data[3];
};
