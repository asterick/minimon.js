#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace Blitter {
	struct __attribute__((packed)) Sprite {
		unsigned x:7;
		unsigned:1;

		unsigned y:7;
		unsigned:1;

		unsigned tile:8;

		unsigned x_flip: 1;
		unsigned y_flip: 1;
		unsigned invert: 1;
		unsigned enable: 1;
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
		uint8_t divider;
		uint8_t counter;
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu, int osc3);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
}
