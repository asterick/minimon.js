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

namespace Input {
	union State {
		uint8_t bytes[6];

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
