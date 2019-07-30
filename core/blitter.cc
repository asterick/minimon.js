#include <string.h>
#include "machine.h"

static const int SCREEN_WIDTH = 96;
static const int SCREEN_HEIGHT = 64;
static const int TICKS_PER_COUNT = 843;
static const int TICK_OVERFLOW = TICKS_PER_COUNT * 72;

union FrameBuffer {
	uint8_t  bytes[SCREEN_WIDTH][8];
	uint64_t column[SCREEN_WIDTH];
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
	0b00000000
};

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

static inline uint16_t rev(uint16_t a) {
	a = ((a & 0b1010101010101010) >> 1) | ((a & 0b0101010101010101) << 1);
	a = ((a & 0b1100110011001100) >> 2) | ((a & 0b0011001100110011) << 2);
	a = ((a & 0b1111000011110000) >> 4) | ((a & 0b0000111100001111) << 4);

	return (a >> 8) | (a << 8);
}

void Blitter::reset(Machine::State& cpu) {
	memset(&cpu.blitter, 0, sizeof(cpu.blitter));
}

void Blitter::clock(Machine::State& cpu, int osc3) {
	bool overflow = false;
	cpu.blitter.counter += osc3;

	while (cpu.blitter.counter >= TICK_OVERFLOW) {
		// Framerate divider
		if (++cpu.blitter.divider >= FRAME_DIVIDERS[cpu.blitter.frame_divider]) {
			cpu.blitter.divider = 0;
			overflow = true;
		}

		cpu.blitter.counter -= TICK_OVERFLOW;
	}

	if (!overflow) return ;

	// Rendering loop
	FrameBuffer target;

	if (cpu.blitter.enable_map)	{
		const MapSize& size = MAP_SIZES[cpu.blitter.map_size];
		int dx = min(cpu.blitter.scroll_x, size.width * 8 - SCREEN_WIDTH);
		int dy = min(cpu.blitter.scroll_y, size.height * 8 - SCREEN_HEIGHT);

		int y_fine = dy % 8;
		int y_tile = (dy / 8) * size.width;

		for (int x = 0; x < SCREEN_WIDTH; x++, dx++) {
			int x_fine = dx % 8;
			int x_tile = dx / 8;
			int address = y_tile + x_tile;

			target.column[x] = 0;

			for(int y = -y_fine, yt; y < SCREEN_HEIGHT; y += 8, address += size.width) {
				uint8_t tile = cpu.overlay.map[address];
				uint8_t graphic = cpu_read8(cpu, cpu.blitter.map_base + x_fine + tile * 8);
				target.column[x] |= shift(graphic, y);
			}
		}

		if (cpu.blitter.invert_map) {
			for (int x = 0; x < SCREEN_WIDTH; x++) target.column[x] = ~target.column[x];
		}
	} else {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			for (int y = 0; y < 8; y++) {
				target.bytes[x][y] = cpu.overlay.framebuffer[y][x];
			}
		}
	}

	if (cpu.blitter.enable_sprites) {
		for (int i = 23; i >= 0; i--) {
			Sprite& sprite = cpu.overlay.oam[i];
		
			if (!sprite.enable) continue ;

			auto address = cpu.blitter.sprite_base + sprite.tile * (8 * 8);
			int dx = sprite.x - 16;
			int dy = sprite.y - 16;

			int invert = sprite.x_flip ? 0b0100111 : 0;

			if (dy <= -16 || dy >= SCREEN_HEIGHT) continue ;

			for (int s = 0; s < 2; s++) {
				for (int x = 0; x < 8; x++, address++, dx++) {
					// Offscreen
					if (dx >= SCREEN_WIDTH) {
						break ;
					} else if (dx < 0) {
						continue;
					}

					uint16_t mask = cpu_read8(cpu, (address^invert) +  0) | (cpu_read8(cpu, (address^invert) +  8) << 8);
					uint16_t draw = cpu_read8(cpu, (address^invert) + 16) | (cpu_read8(cpu, (address^invert) + 24) << 8);

					if (sprite.y_flip) {
						mask = rev(mask);
						draw = rev(draw);
					}

					if (sprite.invert) {
						draw = ~draw;
					}

					mask = ~mask;
					target.column[dx] &= ~shift(mask, dy);
					target.column[dx] |= shift(draw & mask, dy);
				}

				address += (8*3);
			}
		}
	}

	// Copy back to ram
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < 8; y++) {
			cpu.overlay.framebuffer[y][x] = target.bytes[x][y];
		}
	}

	// Send to LCD
	if (cpu.blitter.enable_copy) {
		IRQ::trigger(cpu, IRQ::IRQ_BLT_COPY);

		for (int p = 0, a = 0; p < 8; p++) {
			LCD::write(cpu, 0b10110000 | p, 0x20FE);
			LCD::write(cpu, 0b00000000, 0x20FE);
			LCD::write(cpu, 0b00010000, 0x20FE);
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				LCD::write(cpu, cpu.ram[a++], 0x20FF);
			}
		}
	} else {
		cpu.lcd.updated = true;
	}

	IRQ::trigger(cpu, IRQ::IRQ_BLT_OVERFLOW);
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
			return cpu.blitter.counter / TICKS_PER_COUNT;
	}

	return 0;
}

#include "debug.h"

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
