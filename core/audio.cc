/*
ISC License

Copyright (c) 2019, Bryon Vandiver

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdint.h>
#include <string.h>

#include "machine.h"

extern "C" void audio_push(float*);

void Audio::reset(Audio::State& audio) {
	memset(&audio, 0, sizeof(audio));
}

void Audio::setSampleRate(Audio::State& audio, int sampleRate) {
	audio.sampleRate = sampleRate;
}

void Audio::clock(Machine::State& state, int osc3) {
	Audio::State& audio = state.audio;

	audio.sampleError += osc3 * audio.sampleRate;

	while (audio.sampleError > OSC3_SPEED) {
		// TODO: GET TOUT FROM TIM4/5 HERE
		audio.output[audio.write_index] = 0.0; // WRITE TOUT
		audio.write_index = (audio.write_index + 1) % AUDIO_BUFFER_LENGTH;
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
