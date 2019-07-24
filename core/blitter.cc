#include <string.h>
#include "machine.h"
#include "debug.h"

union FrameBuffer {
	uint8_t  bytes[96][8];
	uint64_t column[96];
};

struct MapSize {
	int width;
	int height;
};

static const MapSize MAP_SIZES[] = {
	{ 12, 16 },
	{ 16, 12 },
	{ 24,  8 },
	{ 24, 16 }
};

static const int FRAME_DIVIDERS[] = {
	3, 6, 9, 12,
	2, 4, 6, 8
};

static const uint8_t BIT_MASK[] = {
	0b00111111,
	0b00001111,
	0b11111000,
	0b11111111,
	0b00011111,
	0b01111111,
	0b01111111,
	0b11000000,
	0b11111111,
	0b00011111,
};

static const int SCREEN_WIDTH = 96;
static const int SCREEN_HEIGHT = 64;

static inline uint64_t shift(uint64_t value, int offset) {
	if (offset < 0) {
		return value >> -offset;
	} else {
		return value << offset;
	}
}

static inline int max(int a, int b) {
	return (a > b) ? a : b;
}

static inline int min(int a, int b) {
	return (a < b) ? a : b;
}

static void render(Machine::State& cpu) {
	FrameBuffer target;

	// Prepare background layer
	if (cpu.blitter.enable_map)	{
		const MapSize& size = MAP_SIZES[cpu.blitter.map_size];
		int dx = min(cpu.blitter.scroll_x, size.width * 8 - SCREEN_WIDTH);
		int dy = min(cpu.blitter.scroll_y, size.height * 8 - SCREEN_HEIGHT);

		int y_fine = dy % 8;
		int y_tile = dy / 8;

		for (int x = 0; x < SCREEN_WIDTH; x++, dx++) {
			int x_fine = dx % 8;
			int x_tile = dx / 8;
			int address = y_tile * size.width + x_tile;

			target.column[x] = 0;

			for(int y = -y_fine, yt; y < SCREEN_HEIGHT; y++, address += size.width) {
				uint8_t graphic = cpu_read8(cpu, cpu.blitter.map_base + cpu.overlay.map[address] + x_fine);
				target.column[x] |= shift(graphic, y);
			}
		}

		if (cpu.blitter.invert_map) {
			for (int x = 0; x < 96; x++) target.column[x] = ~target.column[x];
		}
	} else {
		for (int x = 0; x < 96; x++) {
			for (int y = 0; y < 8; y++) {
				target.bytes[x][y] = cpu.overlay.framebuffer[y][x];
			}
		}
	}

	if (cpu.blitter.enable_sprites) {
		// TODO: DRAW ALL SPRITES		
	}

	// Copy back to ram
	for (int x = 0; x < 96; x++) {
		for (int y = 0; y < 8; y++) {
			cpu.overlay.framebuffer[y][x] = target.bytes[x][y];
		}
	}

	// TODO: SEND TO LCD
}

void Blitter::reset(Machine::State& cpu) {
	memset(&cpu.blitter, 0, sizeof(cpu.blitter));
}

void Blitter::clock(Machine::State& cpu, int osc3) {
	// TODO: Frame rate control
}

uint8_t Blitter::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2080:
			return cpu.blitter.enables;
		case 0x2081:
			return cpu.blitter.rate_control | (cpu.blitter.divider << 4);
		case 0x2082:
			return cpu.blitter.map_bytes[0];
		case 0x2083:
			return cpu.blitter.map_bytes[1];
		case 0x2084:
			return cpu.blitter.map_bytes[2];
		case 0x2085:
			return cpu.blitter.scroll_y;
		case 0x2086:
			return cpu.blitter.scroll_x;
		case 0x2087:
			return cpu.blitter.sprite_bytes[0];
		case 0x2088:
			return cpu.blitter.sprite_bytes[1];
		case 0x2089:
			return cpu.blitter.sprite_bytes[2];
		case 0x208A:
			return cpu.blitter.counter ;
	}

	return 0;
}

void Blitter::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	data &= BIT_MASK[address & 0xF];

	switch (address) {
		case 0x2080:
			cpu.blitter.enables = data;
			break ;
		case 0x2081:
			cpu.blitter.rate_control = data;
			break ;
		case 0x2082:
			cpu.blitter.map_bytes[0] = data;
			break ;
		case 0x2083:
			cpu.blitter.map_bytes[1] = data;
			break ;
		case 0x2084:
			cpu.blitter.map_bytes[2] = data;
			break ;
		case 0x2085:
			cpu.blitter.scroll_y = data;
			break ;
		case 0x2086:
			cpu.blitter.scroll_x = data;
			break ;
		case 0x2087:
			cpu.blitter.sprite_bytes[0] = data;
			break ;
		case 0x2088:
			cpu.blitter.sprite_bytes[1] = data;
			break ;
		case 0x2089:
			cpu.blitter.sprite_bytes[2] = data;
			break ;
	}
}
