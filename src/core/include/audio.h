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

static const int AUDIO_BUFFER_LENGTH = 1024;

namespace Audio {
	struct State {
		uint8_t volume;
		uint8_t enable;

		float output[AUDIO_BUFFER_LENGTH];
		int write_index;

		int sampleRate;
		int sampleAccumulator;
		int sampleCount;
		int sampleError;
	};

	void setSampleRate(State&, int sampleRate);
	float* getSampleBuffer(Audio::State& audio);

	void reset(State&);
	void clock(Machine::State&, int osc3);
	uint8_t read(State&, uint32_t address);
	void write(State&, uint8_t data, uint32_t address);
};
