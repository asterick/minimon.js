#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Timer {
	struct State {
		union {
			uint8_t status_bytes[6];
			struct   __attribute__((packed)) {
				unsigned tim0_scalar:3;
				unsigned tim0_enable:1;
				unsigned tim1_scalar:3;
				unsigned tim1_enable:1;
				unsigned tim0_oscillator:1;
				unsigned tim1_oscillator:1;
				unsigned:2;
				unsigned osc1_enable:1;
				unsigned osc3_enable:1;
				unsigned:2;

				unsigned tim2_scalar:3;
				unsigned tim2_enable:1;
				unsigned tim3_scalar:3;
				unsigned tim3_enable:1;
				unsigned tim2_oscillator:1;
				unsigned tim3_oscillator:1;
				unsigned:6;

				unsigned tim4_scalar:3;
				unsigned tim4_enable:1;
				unsigned tim5_scalar:3;
				unsigned tim5_enable:1;
				unsigned tim4_oscillator:1;
				unsigned tim5_oscillator:1;
			};
		};

		uint32_t osc1_prescale;
		uint32_t osc3_prescale;
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu, int osc1, int osc3);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
};
