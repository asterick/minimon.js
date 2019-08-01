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

using namespace IRQ;

struct VectorTable {
	bool maskable;
	int priority_group;
	int bit_group;
};

static const VectorTable IRQ_TABLE[TOTAL_HARDWARE_IRQS] = {
	{ false },			// IRQ_RESET
	{ false },			// IRQ_DIV_ZERO
	{ false },			// IRQ_WATCHDOG
	{ true,   6, 007 },	// IRQ_BLT_COPY
	{ true,   6, 006 },	// IRQ_BLT_OVERFLOW
	{ true,   4, 005 },	// IRQ_TIM3
	{ true,   4, 004 },	// IRQ_TIM2
	{ true,   2, 003 },	// IRQ_TIM1
	{ true,   2, 002 },	// IRQ_TIM0
	{ true,   0, 001 },	// IRQ_TIM5
	{ true,   0, 000 },	// IRQ_TIM5_CMP
	{ true,  14, 015 },	// IRQ_32HZ
	{ true,  14, 014 },	// IRQ_8HZ
	{ true,  14, 013 },	// IRQ_2HZ
	{ true,  14, 012 },	// IRQ_1HZ
	{ true,  16, 037 },	// IRQ_IR_RCV
	{ true,  16, 036 },	// IRQ_SHOCK
	{ true, 000, 035 },	// IRQ_UNKNOWN1
	{ true, 000, 034 },	// IRQ_UNKNOWN2
	{ true,  12, 011 },	// IRQ_K09
	{ true,  12, 010 },	// IRQ_K08
	{ true,  10, 027 },	// IRQ_K07
	{ true,  10, 026 },	// IRQ_K06
	{ true,  10, 025 },	// IRQ_K05
	{ true,  10, 024 },	// IRQ_K04
	{ true,  10, 023 },	// IRQ_K03
	{ true,  10, 022 },	// IRQ_K02
	{ true,  10, 021 },	// IRQ_K01
	{ true,  10, 020 },	// IRQ_K00
	{ true, 000, 032 },	// IRQ_UNKNOWN3
	{ true, 000, 031 },	// IRQ_UNKNOWN4
	{ true, 000, 030 }	// IRQ_UNKNOWN5
};

// These are the write masks for the data pins (no mapped d-latch)
static const uint8_t BIT_MASK[] = {
	0b11111111,
	0b11111111,
	0b00000011,

	0b11111111,
	0b00111111,
	0b11111111,
	0b11110111
};

static void refresh_irqs(Machine::State& cpu) {
	cpu.irq.next_priority = 0;

	for (int irq = FIRST_MASKABLE; irq < TOTAL_HARDWARE_IRQS; irq++) {
		const VectorTable& info = IRQ_TABLE[irq];

		bool active = (1 << info.bit_group) & cpu.irq.active & cpu.irq.enable;

		// Is IRQ active and enabled?
		if (!active) {
			continue ;
		}

		int priority = (cpu.irq.priority >> info.priority_group) & HIGHEST_PRIO;

		if (cpu.irq.next_priority < priority) {
			cpu.irq.next_priority = priority;
			cpu.irq.next_irq = (Vector) irq;
		}
	}
}

void IRQ::reset(Machine::State& cpu) {
	cpu.irq.priority = 0;
	cpu.irq.enable = 0;
	cpu.irq.active = 0;

	refresh_irqs(cpu);
}

void IRQ::trigger(Machine::State& cpu, Vector irq) {
	const VectorTable& info = IRQ_TABLE[irq];

	if (irq < FIRST_MASKABLE) {
		cpu.irq.next_priority = HIGHEST_PRIO;
		cpu.irq.next_irq = irq;
	} else {
		cpu.irq.active |= 1 << info.bit_group;
		refresh_irqs(cpu);
	}
}

uint8_t IRQ::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2020: return cpu.irq.priority_bytes[0];
		case 0x2021: return cpu.irq.priority_bytes[1];
		case 0x2022: return cpu.irq.priority_bytes[2];
		case 0x2023: return cpu.irq.enable_bytes[0];
		case 0x2024: return cpu.irq.enable_bytes[1];
		case 0x2025: return cpu.irq.enable_bytes[2];
		case 0x2026: return cpu.irq.enable_bytes[3];
		case 0x2027: return cpu.irq.active_bytes[0];
		case 0x2028: return cpu.irq.active_bytes[1];
		case 0x2029: return cpu.irq.active_bytes[2];
		case 0x202A: return cpu.irq.active_bytes[3];
	}
	return 0xFF;
}

void IRQ::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	switch (address) {
		case 0x2020: cpu.irq.priority_bytes[0] = data & BIT_MASK[0];
		case 0x2021: cpu.irq.priority_bytes[1] = data & BIT_MASK[1];
		case 0x2022: cpu.irq.priority_bytes[2] = data & BIT_MASK[2];
		case 0x2023: cpu.irq.enable_bytes[0] = data & BIT_MASK[3];
		case 0x2024: cpu.irq.enable_bytes[1] = data & BIT_MASK[4];
		case 0x2025: cpu.irq.enable_bytes[2] = data & BIT_MASK[5];
		case 0x2026: cpu.irq.enable_bytes[3] = data & BIT_MASK[6];
		case 0x2027: cpu.irq.active_bytes[0] &= ~data;
		case 0x2028: cpu.irq.active_bytes[1] &= ~data;
		case 0x2029: cpu.irq.active_bytes[2] &= ~data;
		case 0x202A: cpu.irq.active_bytes[3] &= ~data;
	}

	refresh_irqs(cpu);
}
