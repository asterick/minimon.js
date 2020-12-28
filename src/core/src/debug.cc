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

#include <stdarg.h>
#include "debug.h"

extern "C" void debug_print(const void* data);

static char OUTPUT_BUFFER[0x1000];

void format_num(char*& output, int value, int radix, bool sign) {
	const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char values[32];
	char *wp = values;

	if (value == 0) {
		*(output++) = '0';
		return ;
	}

	if (sign && value < 0) {
		value = -value;
		*(output++) = '-';
	}

	while (value > 0) {
		*(wp++) = digits[value % radix];
		value /= radix;
	}

	while (wp > values) {
		*(output++) = *(--wp);
	}
}

void format_arg(char code, char*& output, int value) {
	switch (code) {
	case 'i':
		format_num(output, value, 10, true);
		break ;
	case 'I':
		format_num(output, value, 10, false);
		break ;
	case 'x':
		format_num(output, value, 16, true);
		break ;
	case 'X':
		format_num(output, value, 16, false);
		break ;
	case 'b':
		format_num(output, value, 2, true);
		break ;
	case 'B':
		format_num(output, value, 2, false);
		break ;
	case 'c':
		*(output++) = (char) value;
		break ;
	}
}

void dprintf(const char* format, ...) {
	char *out = OUTPUT_BUFFER;

	va_list argp;

	va_start(argp, format);
	for (;;) {
		char ch = *(format++);

		if (ch == '%') {
			format_arg(*(format++), out, va_arg(argp, int));
		} else if (ch) {
			*(out++) = ch;
		} else {
			break ;
		}
	}
	va_end(argp);

	*out = 0;
	debug_print(OUTPUT_BUFFER);
}
