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
