#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Control {
	struct State {
		uint8_t data[3];

		bool lcd_enabled;
		bool cart_enabled;
	};

	void reset(State&);
	uint8_t read(State&, uint32_t address);
	void write(State&, uint8_t data, uint32_t address);
};
