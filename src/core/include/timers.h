/*
 * MIT License
 *
 * Copyright (c) 2019-2021, Bryon Vandiver
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Timers {
	union Timer {
		uint8_t bytes[8];

		struct  __attribute__((packed)) {
			unsigned lo_input:1;
			unsigned lo_preset:1;
			unsigned lo_running:1;
			unsigned lo_output:1;
			unsigned:3;
			unsigned mode16:1;

			unsigned hi_input:1;
			unsigned hi_preset:1;
			unsigned hi_running:1;
			unsigned hi_output:1;
			unsigned:4;

			union {
				uint16_t preset;
				uint8_t preset_bytes[2];
			};
			union {
				uint16_t compare;
				uint8_t compare_bytes[2];
			};
			union {
				uint16_t count;
				uint8_t count_bytes[2];
			};

			int  lo_clock_ratio;
			bool lo_clock_ctrl;
			bool lo_clock_source;

			int  hi_clock_ratio;
			bool hi_clock_ctrl;
			bool hi_clock_source;
		};
	};

	struct State {
		Timer timer[3];

		bool osc1_enable;
		bool osc3_enable;

		uint32_t osc1_prescale;
		uint32_t osc3_prescale;
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu, int osc1, int osc3);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
};
