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

		int32_t overflow;
	};

	void reset(LCD::State& lcd);

	uint8_t get_scanline(LCD::State& lcd);
	void clock(Machine::State& cpu, int osc1);
	uint8_t read(LCD::State& lcd, uint32_t address);
	void write(LCD::State& lcd, uint8_t data, uint32_t address);
}
