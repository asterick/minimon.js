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

#include <stdint.h>
#include <string.h>

#include "machine.h"

#include "debug.h"

extern "C" void audio_push(float*, int frames);

void Audio::reset(Audio::State& audio) {
	audio.enable = 0;
	audio.volume = 0;
	audio.write_index = 0;
	memset(&audio.output, 0, sizeof(audio.output));
}

void Audio::setSampleRate(Audio::State& audio, int sampleRate) {
	audio.sampleRate = sampleRate;
}

void Audio::clock(Machine::State& state, int osc3) {
	Audio::State& audio = state.audio;

	audio.sampleError += osc3 * audio.sampleRate;
	while (audio.sampleError > OSC3_SPEED) {
		Timers::Timer& timer = state.timers.timer[2];
		float volume;

		if (!audio.enable) {
			switch (audio.volume) {
				case 0b000: case 0b100:
					volume = 0.0;
					break ;
				default:
					volume = 0.5;
					break ;
				case 0b011: case 0b111:
					volume = 1.0;
					break ;
			}

			if (timer.count < timer.compare) {
				volume = -volume;
			}
		} else {
			volume = 0.0;
		}


		audio.output[audio.write_index++] = volume;

		if (audio.write_index >= AUDIO_BUFFER_LENGTH) {
			audio_push(audio.output, AUDIO_BUFFER_LENGTH);
			audio.write_index = 0;
		}

		audio.sampleError -= OSC3_SPEED;
	}
}

uint8_t Audio::read(Audio::State& audio, uint32_t address) {
	switch (address) {
		case 0x2070:
			return audio.enable;
		case 0x2071:
			return audio.volume;
	}
	return 0;
}

void Audio::write(Audio::State& audio, uint8_t data, uint32_t address) {
	switch (address) {
		case 0x2070:
			audio.enable = data & 0b111;
			break ;
		case 0x2071:
			audio.volume = data & 0b111;
			break ;
	}
}
