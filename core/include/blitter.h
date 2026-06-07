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
#include <stddef.h>

namespace Machine { struct State; };

namespace Blitter {
	// Sprite flag bits (byte 3 of an OAM entry)
	const uint8_t SPRITE_X_FLIP = 0x01;
	const uint8_t SPRITE_Y_FLIP = 0x02;
	const uint8_t SPRITE_INVERT = 0x04;
	const uint8_t SPRITE_ENABLE = 0x08;

	// A view over a 4-byte guest OAM entry: x, y (7 bits each), tile, flags
	struct Sprite {
		uint8_t bytes[4];

		int x() const {
			return bytes[0] & 0x7F;
		}

		int y() const {
			return bytes[1] & 0x7F;
		}

		int tile() const {
			return bytes[2];
		}

		bool x_flip() const {
			return (bytes[3] & SPRITE_X_FLIP) != 0;
		}

		bool y_flip() const {
			return (bytes[3] & SPRITE_Y_FLIP) != 0;
		}

		bool invert() const {
			return (bytes[3] & SPRITE_INVERT) != 0;
		}

		bool enable() const {
			return (bytes[3] & SPRITE_ENABLE) != 0;
		}
	};

	struct Overlay {
		uint8_t framebuffer[8][96];
		Sprite  oam[24];
		// Remainder of the 4KB RAM window (flexible array members inside
		// a union arm are not portable C++)
		uint8_t map[0x1000 - sizeof(uint8_t[8][96]) - sizeof(Sprite[24])];
	};

	// Overlay is a view over the guest's 4KB RAM window (it shares a
	// union with Machine::State::ram); pin the layout so a compiler
	// that pads these structs fails loudly instead of corrupting state
	static_assert(sizeof(Sprite) == 4, "OAM entries are 4 guest bytes");
	static_assert(sizeof(Overlay) == 0x1000, "Overlay must span the RAM window exactly");
	static_assert(offsetof(Overlay, oam) == 768, "OAM must directly follow the framebuffer");
	static_assert(offsetof(Overlay, map) == 864, "map data must directly follow OAM");

	// enables bits (0x2080)
	const uint8_t ENABLES_INVERT_MAP     = 0x01;
	const uint8_t ENABLES_MAP            = 0x02;
	const uint8_t ENABLES_SPRITES        = 0x04;
	const uint8_t ENABLES_COPY           = 0x08;
	const uint8_t ENABLES_MAP_SIZE_MASK  = 0x30;
	const int     ENABLES_MAP_SIZE_SHIFT = 4;

	// rate_control bits (0x2081); bit 0 is an unknown flag (possibly a
	// frame counter enable), the upper nibble reads back the divider
	const uint8_t RATE_DIVIDER_MASK  = 0x0E;
	const int     RATE_DIVIDER_SHIFT = 1;

	struct State {
		uint8_t enables;
		uint8_t rate_control;

		// Tile data addresses; the guest programs the low three bytes
		// through 0x2082-4 / 0x2087-9, the high byte stays zero
		uint32_t map_base;
		uint32_t sprite_base;

		uint8_t scroll_x;
		uint8_t scroll_y;

		// Counters
		uint8_t divider;

		bool invert_map() const {
			return (enables & ENABLES_INVERT_MAP) != 0;
		}

		bool enable_map() const {
			return (enables & ENABLES_MAP) != 0;
		}

		bool enable_sprites() const {
			return (enables & ENABLES_SPRITES) != 0;
		}

		bool enable_copy() const {
			return (enables & ENABLES_COPY) != 0;
		}

		int map_size() const {
			return (enables & ENABLES_MAP_SIZE_MASK) >> ENABLES_MAP_SIZE_SHIFT;
		}

		int frame_divider() const {
			return (rate_control & RATE_DIVIDER_MASK) >> RATE_DIVIDER_SHIFT;
		}

		static uint8_t base_byte(uint32_t base, int index) {
			return base >> (8 * index);
		}

		static void set_base_byte(uint32_t& base, int index, uint8_t value) {
			const int shift = 8 * index;
			base = (base & ~(0xFFu << shift)) | ((uint32_t)value << shift);
		}
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
}
