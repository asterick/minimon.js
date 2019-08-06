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

const auto LCD_SPEED	= 72 * 0x41;	// Line scan frequency

namespace LCD {
	struct State {
		uint8_t gddram[9][132];
		uint8_t read_buffer;
		uint8_t volume;
		uint8_t column_address;
		uint8_t page_address;
		uint8_t start_address;

		bool	rmw_mode;
		bool 	adc_select;
		bool	setting_volume;
		bool	display_enable;
		bool	reverse_display;
		bool	all_on;
		bool	reverse_com_scan;
		bool	static_indicator;
		bool	lcd_bias;
		uint8_t	resistor_ratio;
		uint8_t	operating_mode;
		uint8_t scanline;

		uint8_t framebuffer[64][96];

		int 	overflow;
	};

	void reset(LCD::State& lcd);

	uint8_t get_scanline(LCD::State& lcd);
	void clock(Machine::State& cpu, int osc3);
	uint8_t read(LCD::State& lcd, uint32_t address);
	void write(LCD::State& lcd, uint8_t data, uint32_t address);
}
