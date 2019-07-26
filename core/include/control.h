#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Control {
	union State {
		uint8_t data[3];

		struct {
			unsigned lcd_enabled:1;
			unsigned cart_enabled:1;
		};
	};

	void reset(State&);
	uint8_t read(State&, uint32_t address);
	void write(State&, uint8_t data, uint32_t address);
};
