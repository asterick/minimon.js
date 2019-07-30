#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Input {
	union State {
		uint8_t input_bytes[6];

		struct __attribute__((packed)) {
			unsigned interrupt_direction:10;
			unsigned:6;
			unsigned input_state:10;
			unsigned:6;
			unsigned dejitter_k00_k03:3;
			unsigned:1;
			unsigned dejitter_k04_k07:3;
			unsigned:1;
			unsigned dejitter_k10_k11:3;
			unsigned:1;
		};
	};

	void reset(Input::State&);
	void update(Machine::State&, uint16_t value);
	uint8_t read(Input::State&, uint32_t address);
	void write(Input::State&, uint8_t data, uint32_t address);
};
