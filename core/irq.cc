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
#include "debug.h"

using namespace IRQ;

struct VectorTable {
	bool maskable;
	int priority_group;
	int bit_group;
};


static const VectorTable IRQ_TABLE[TOTAL_HARDWARE_IRQS] = {
	{ false },			// 0x00 IRQ_RESET
	{ false },			// 0x01 IRQ_DIV_ZERO
	{ false },			// 0x02 IRQ_WATCHDOG
	{ true, 006, 007 },	// 0x03 IRQ_BLT_COPY
	{ true, 006, 006 },	// 0x04 IRQ_BLT_OVERFLOW
	{ true, 004, 005 },	// 0x05 IRQ_TIM3
	{ true, 004, 004 },	// 0x06 IRQ_TIM2
	{ true, 002, 003 },	// 0x07 IRQ_TIM1
	{ true, 002, 002 },	// 0x08 IRQ_TIM0
	{ true, 000, 001 },	// 0x09 IRQ_TIM5
	{ true, 000, 000 },	// 0x0A IRQ_TIM5_CMP
	{ true, 016, 015 },	// 0x0B IRQ_32HZ
	{ true, 016, 014 },	// 0x0C IRQ_8HZ
	{ true, 016, 013 },	// 0x0D IRQ_2HZ
	{ true, 016, 012 },	// 0x0E IRQ_1HZ
	{ true, 020, 037 },	// 0x0F IRQ_IR_RCV
	{ true, 020, 036 },	// 0x10 IRQ_SHOCK
	{ true, 000, 035 },	// 0x11 IRQ_UNKNOWN1
	{ true, 000, 034 },	// 0x12 IRQ_UNKNOWN2
	{ true, 014, 011 },	// 0x13 IRQ_K09
	{ true, 014, 010 },	// 0x14 IRQ_K08
	{ true, 012, 027 },	// 0x15 IRQ_K07
	{ true, 012, 026 },	// 0x16 IRQ_K06
	{ true, 012, 025 },	// 0x17 IRQ_K05
	{ true, 012, 024 },	// 0x18 IRQ_K04
	{ true, 012, 023 },	// 0x19 IRQ_K03
	{ true, 012, 022 },	// 0x1A IRQ_K02
	{ true, 012, 021 },	// 0x1B IRQ_K01
	{ true, 012, 020 },	// 0x1C IRQ_K00
	{ true, 010, 032 },	// 0x1D IRQ_UNKNOWN3
	{ true, 010, 031 },	// 0x1E IRQ_UNKNOWN4
	{ true, 010, 030 }	// 0x1F IRQ_UNKNOWN5
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

static inline void fire(Machine::State& cpu, Vector irq, int priority) {
	cpu.halted = false;

	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu_clock(cpu, 7);
	cpu.reg.pc = cpu_read16(cpu, 2 * (int) irq);
	cpu.reg.flag.i = priority;
}

void IRQ::manage(Machine::State& cpu) {
	if (cpu.reg.flag.i >= cpu.irq.next_priority) {
		return ;
	}

	fire(cpu, cpu.irq.next_irq, cpu.irq.next_priority);
}

void IRQ::trigger(Machine::State& cpu, Vector irq) {
	const VectorTable& info = IRQ_TABLE[irq];

	if (!info.maskable) {
		fire(cpu, irq, HIGHEST_PRIO);
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
