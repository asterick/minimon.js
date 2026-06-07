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

#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Input {
	// Register file at 0x2050: bytes 0-1 are the 10-bit interrupt edge
	// directions, bytes 2-3 the 10-bit pad state (both little-endian),
	// bytes 4-5 hold the dejitter constants (K00-K03, K04-K07, K10-K11)
	struct State {
		uint8_t bytes[6];

		uint16_t interrupt_direction() const {
			return bytes[0] | ((bytes[1] & 0x03) << 8);
		}

		uint16_t input_state() const {
			return bytes[2] | ((bytes[3] & 0x03) << 8);
		}

		void set_input_state(uint16_t value) {
			bytes[2] = (uint8_t)value;
			bytes[3] = (value >> 8) & 0x03;
		}
	};

	void reset(Input::State&);
	void update(Machine::State&, uint16_t value);
	uint8_t read(Input::State&, uint32_t address);
	void write(Input::State&, uint8_t data, uint32_t address);
};
