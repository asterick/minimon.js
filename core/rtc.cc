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

#include "machine.h"

void RTC::reset(Machine::State& cpu) {
	cpu.rtc.running = false;
	cpu.rtc.value = 0;
}

void RTC::clock(Machine::State& cpu, int osc1) {
	if (cpu.rtc.running) {
		cpu.rtc.value += osc1;
	}
}

uint8_t RTC::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2008: return cpu.rtc.running ? 0b1 :0b0;
		case 0x2009: return cpu.rtc.value >> 15;
		case 0x200A: return cpu.rtc.value >> 23;
		case 0x200B: return cpu.rtc.value >> 31;
	}

	return 0;
}

void RTC::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	if (address == 0x2008) {
		cpu.rtc.running = (data & 0b01);

		if (data & 0b10) {
			cpu.rtc.value = 0;
		}
	}
}
