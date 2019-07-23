#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Blitter {
	struct State {
		union {
			unsigned int map_base;
			uint8_t map_bytes[3];
		};
		union {
			unsigned int sprite_base;
			uint8_t sprite_bytes[3];
		};
		uint8_t scroll_x;
		uint8_t scroll_y;
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu, int osc3);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
}
