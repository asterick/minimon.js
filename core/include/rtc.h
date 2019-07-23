#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace RTC {
	struct State {
		bool running;
		uint64_t value;
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu, int osc3);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
}
