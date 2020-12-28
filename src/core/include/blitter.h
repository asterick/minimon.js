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

namespace Blitter {
	union Sprite {
		uint8_t bytes[4];
		struct {
			unsigned x:7;
			unsigned:1;

			unsigned y:7;
			unsigned:1;

			unsigned tile:8;

			unsigned x_flip: 1;
			unsigned y_flip: 1;
			unsigned invert: 1;
			unsigned enable: 1;
			unsigned:4;
		};
	};

	struct Overlay {
		uint8_t framebuffer[8][96];
		Sprite  oam[24];
		uint8_t map[];
	};

	struct State {
		union {
			uint8_t enables;
			struct {
				unsigned invert_map:1;
				unsigned enable_map:1;
				unsigned enable_sprites:1;
				unsigned enable_copy:1;
				unsigned map_size:2;
			};
		};
		union {
			uint8_t rate_control;
			struct {
				unsigned:1;	// Unknown flag: Possible frame counter enable?
				unsigned frame_divider:3;
				unsigned frame_count:4;
			};
		};

		union {
			unsigned int map_base;
			uint8_t map_bytes[3];
		};
		union {
			unsigned int sprite_base;
			uint8_t sprite_bytes[3];
		};

		uint8_t scroll_x;
		uint8_t scroll_y;

		// Counters
		uint8_t divider;
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
}
