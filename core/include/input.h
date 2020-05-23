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
