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

#include <stdint.h>

#include "machine.h"

void RTC::reset(Machine::State& cpu) {
	cpu.rtc.running = false;
	cpu.rtc.value = 0;
	cpu.rtc.prescale = 0;
}

void RTC::clock(Machine::State& cpu, int osc1) {
	if (cpu.rtc.running) {
		cpu.rtc.prescale += osc1;

		while (cpu.rtc.prescale >= 0x8000)	 {
			cpu.rtc.value ++;
			cpu.rtc.prescale -= cpu.rtc.prescale;
		}
	}
}

uint8_t RTC::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2008: return cpu.rtc.running ? 0b1 :0b0;
		case 0x2009: return cpu.rtc.value;
		case 0x200A: return cpu.rtc.value >> 8;
		case 0x200B: return cpu.rtc.value >> 16;
	}

	return 0;
}

void RTC::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	if (address == 0x2008) {
		cpu.rtc.running = (data & 0b01);

		if (data & 0b10) {
			cpu.rtc.value = 0;
			cpu.rtc.prescale = 0;
		}
	}
}
