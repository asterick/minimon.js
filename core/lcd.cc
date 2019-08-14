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
#include "debug.h"

static const int LCD_WIDTH = 96;
static const int LCD_HEIGHT = 64;

struct ContrastSetting {
	uint8_t on;
	uint8_t off;
};

static const ContrastSetting levels[0x40] = {
	{ 0xFF, 0xFF },	{ 0xF7, 0xFF },	{ 0xEF, 0xFF },	{ 0xE7, 0xFF },
	{ 0xDE, 0xFF },	{ 0xD6, 0xFF },	{ 0xCE, 0xFF },	{ 0xC6, 0xFF },
	{ 0xBD, 0xFF },	{ 0xB5, 0xFF },	{ 0xAD, 0xFF },	{ 0xA5, 0xFF },
	{ 0x9C, 0xFF },	{ 0x94, 0xFF },	{ 0x8C, 0xFF },	{ 0x84, 0xFF },
	{ 0x7B, 0xFF },	{ 0x73, 0xFF },	{ 0x6B, 0xFF },	{ 0x63, 0xFF },
	{ 0x5A, 0xFF },	{ 0x52, 0xFF },	{ 0x4A, 0xFF },	{ 0x42, 0xFF },
	{ 0x39, 0xFF },	{ 0x31, 0xFF },	{ 0x29, 0xFF },	{ 0x21, 0xFF },
	{ 0x18, 0xFF },	{ 0x10, 0xFF },	{ 0x08, 0xFF },	{ 0x00, 0xFF },
	{ 0x00, 0xFF },	{ 0x00, 0xF7 },	{ 0x00, 0xEF },	{ 0x00, 0xE7 },
	{ 0x00, 0xDE },	{ 0x00, 0xD6 },	{ 0x00, 0xCE },	{ 0x00, 0xC6 },
	{ 0x00, 0xBD },	{ 0x00, 0xB5 },	{ 0x00, 0xAD },	{ 0x00, 0xA5 },
	{ 0x00, 0x9C },	{ 0x00, 0x94 },	{ 0x00, 0x8C },	{ 0x00, 0x84 },
	{ 0x00, 0x7B },	{ 0x00, 0x73 },	{ 0x00, 0x6B },	{ 0x00, 0x63 },
	{ 0x00, 0x5A },	{ 0x00, 0x52 },	{ 0x00, 0x4A },	{ 0x00, 0x42 },
	{ 0x00, 0x39 },	{ 0x00, 0x31 },	{ 0x00, 0x29 },	{ 0x00, 0x21 },
	{ 0x00, 0x18 },	{ 0x00, 0x10 },	{ 0x00, 0x08 },	{ 0x00, 0x00 }
};

void LCD::reset(LCD::State& lcd) {
	memset(&lcd, 0, sizeof(lcd));
}

extern "C" void flip_screen(void*);

static void renderline(LCD::State& lcd, int com) {
	const uint8_t off = levels[lcd.volume].off;
	const uint8_t on = levels[lcd.volume].on;

	uint8_t* line = lcd.framebuffer[lcd.reverse_com_scan ? (63 - com) : com];

	if (!lcd.display_enable) {
		memset(line, 0xFF, sizeof(lcd.framebuffer[0]));
		return ;
	} else if (lcd.all_on) {
		memset(line, on, sizeof(lcd.framebuffer[0]));
		return ;
	}

	int drawline = (com + lcd.start_address) % 0x40;
	uint8_t mask = 1 << (drawline % 8);
	uint8_t* current_page = lcd.gddram[drawline / 8];

	for (int x = 0; x < LCD_WIDTH; x++) {
		uint8_t byte =  current_page[lcd.adc_select ? 131 - x : x];
		*(line++) = (byte & mask) ? on : off;
	}
}

void LCD::clock(Machine::State& cpu, int osc3) {
	cpu.lcd.overflow += osc3 * LCD_SPEED;

	while (cpu.lcd.overflow >= OSC3_SPEED) {
		if (cpu.lcd.scanline >= 0x40) {
			flip_screen(cpu.lcd.framebuffer);

			cpu.lcd.scanline = 0;
			Blitter::clock(cpu);
		} else {
			renderline(cpu.lcd, cpu.lcd.scanline++);
		}

		cpu.lcd.overflow -= OSC3_SPEED;
	}
}

uint8_t LCD::get_scanline(LCD::State& lcd) {
	return lcd.scanline + 1;
}

uint8_t LCD::read(LCD::State& lcd, uint32_t address) {
	if (address == 0x20FE) {
		dprintf("READ DISPLAY STATUS");
		return 0;
	} else {
		uint8_t data = lcd.read_buffer;
		
		data = lcd.gddram[lcd.page_address][lcd.column_address];

		if (lcd.column_address < 0x83 && !lcd.rmw_mode) {
			lcd.column_address++;
		}

		return data;
	}
}

void LCD::write(LCD::State& lcd, uint8_t data, uint32_t address) {
	lcd.read_buffer = data;

	if (lcd.setting_volume) {
		lcd.volume = data & 0x3F;
		lcd.setting_volume = false;
		return ;
	}

	if (address == 0x20FE) {
		switch (data) {
		case 0b10101110 ... 0b10101111:
			lcd.display_enable = data & 1;
			break ;

		case 0b01000000 ... 0b01111111:
			lcd.start_address = data & 0b111111;
			break ;

		case 0b00000000 ... 0b00001111:
			lcd.column_address = (lcd.column_address & 0xF0) | (data & 0xF);

			if (lcd.column_address > 0x83) lcd.column_address = 0x83;
			break ;

		case 0b00010000 ... 0b00011111:
			lcd.column_address = (lcd.column_address & 0x0F) | (data << 4);

			if (lcd.column_address > 0x83) lcd.column_address = 0x83;
			break ;

		case 0b00100000 ... 0b00100111:
			lcd.resistor_ratio = data & 0b111;
			break ;

		case 0b00101000 ... 0b00101111:
			lcd.operating_mode = data & 0b111;
			break ;

		case 0b10110000 ... 0b10111111:
			lcd.page_address = data & 0xF;

			if (lcd.page_address > 8) lcd.page_address = 8;
			break ;

		case 0b10100000 ... 0b10100001:
			lcd.adc_select = data & 1;
			break ;

		case 0b10100110 ... 0b10100111:
			lcd.reverse_display = data & 1;
			break ;

		case 0b10100100 ... 0b10100101:
			lcd.all_on = data & 1;
			break ;

		case 0b10100010 ... 0b10100011:
			lcd.lcd_bias = data & 1;
			break ;

		case 0b10101100 ... 0b10101101:
			lcd.static_indicator = data & 1;
			break ;

		case 0b11100000: // Set RMW mode
			lcd.rmw_mode = true;
			break ;

		case 0b11101110: // End
			lcd.rmw_mode = false;
			break ;

		case 0b11000000 ... 0b11001111:
			lcd.reverse_com_scan = data & 8;
			break ;

		case 0b10000001: // Set electric volume
			lcd.setting_volume = true;
			break ;
		
		case 0b11100011: // NOP
			break ;

		default:
			dprintf("LCD COMMAND %b", data);
		}
	} else {
		if (lcd.page_address >= 8) data &= 1;

		lcd.gddram[lcd.page_address][lcd.column_address] = data;

		if (lcd.column_address < 0x83) {
			lcd.column_address++;
		}
	}
}
