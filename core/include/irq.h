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

namespace Machine { struct State; };

namespace IRQ {
	static const int TOTAL_PRIORITY_GROUPS = 9;

	enum Vector : uint8_t {
		IRQ_RESET,			// 0x00
		IRQ_DIV_ZERO,		// 0x01
		IRQ_WATCHDOG,		// 0x02
		IRQ_BLT_COPY,		// 0x03
		IRQ_BLT_OVERFLOW,	// 0x04
		IRQ_TIM3,			// 0x05
		IRQ_TIM2,			// 0x06
		IRQ_TIM1,			// 0x07
		IRQ_TIM0,			// 0x08
		IRQ_TIM5,			// 0x09
		IRQ_TIM5_CMP,		// 0x0A
		IRQ_32HZ,			// 0x0B
		IRQ_8HZ,			// 0x0C
		IRQ_2HZ,			// 0x0D
		IRQ_1HZ,			// 0x0E
		IRQ_IR_RCV,			// 0x0F
		IRQ_SHOCK,			// 0x11
		IRQ_UNKNOWN1,		// 0x11
		IRQ_UNKNOWN2,		// 0x12
		IRQ_K11,			// 0x13
		IRQ_K10,			// 0x14
		IRQ_K07,			// 0x15
		IRQ_K06,			// 0x16
		IRQ_K05,			// 0x17
		IRQ_K04,			// 0x18
		IRQ_K03,			// 0x19
		IRQ_K02,			// 0x1A
		IRQ_K01,			// 0x1B
		IRQ_K00,			// 0x1C
		IRQ_UNKNOWN3,		// 0x1D
		IRQ_UNKNOWN4,		// 0x1E
		IRQ_UNKNOWN5,		// 0x1F

		TOTAL_HARDWARE_IRQS,
		FIRST_MASKABLE = 3
	};

	struct State {
		bool enable[TOTAL_HARDWARE_IRQS];
		bool active[TOTAL_HARDWARE_IRQS];
		uint8_t priority[TOTAL_PRIORITY_GROUPS];

		int next_priority;
		IRQ::Vector next_irq;
	};

	static const uint8_t HIGHEST_PRIO = 3;

	void reset(Machine::State& cpu);
	void manage(Machine::State& cpu);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
	void trigger(Machine::State& cpu, IRQ::Vector irq);
};
