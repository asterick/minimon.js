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

#include <string.h>
#include <stdint.h>

#include "machine.h"
#include "debug.h"

using namespace IRQ;

int irq_group[] = {
	0, 0, 0,				// NMI

	0, 0,					// Blitter Group
	1, 1,					// Tim3/2
	2, 2,					// Tim1/0
	3, 3,					// Tim5/4
	4, 4, 4, 4,				// 256hz clock
	8, 8, 8, 8,				// IR / Shock sensor
	5, 5,					// K1x
	6, 6, 6, 6, 6, 6, 6, 6, // K0x
	7, 7, 7,				// Unknown ($1D ~ $1F?)
};

static void refresh_irqs(Machine::State& cpu) {
	cpu.irq.next_priority = 0;

	for (int irq = FIRST_MASKABLE; irq < TOTAL_HARDWARE_IRQS; irq++) {
		if (!cpu.irq.enable[irq] || !cpu.irq.active[irq]) continue ;

		uint8_t priority = cpu.irq.priority[irq_group[irq]];

		if (cpu.irq.next_priority < priority) {
			cpu.irq.next_priority = priority;
			cpu.irq.next_irq = (Vector) irq;
		}
	}
}

void IRQ::reset(Machine::State& cpu) {
	memset(&cpu.irq, 0, sizeof(cpu.irq));
}

static inline void fire(Machine::State& cpu, Vector irq, uint8_t priority) {
	cpu.status = Machine::STATUS_NORMAL;

	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu_clock(cpu, 7);
	cpu.reg.pc = cpu_read16(cpu, 2 * (int) irq);
	cpu.reg.flag.i = priority;
}

void IRQ::manage(Machine::State& cpu) {
	if (cpu.reg.flag.i < cpu.irq.next_priority) {
		fire(cpu, cpu.irq.next_irq, cpu.irq.next_priority);
	}
}

void IRQ::trigger(Machine::State& cpu, Vector irq) {
	if (irq < FIRST_MASKABLE) {
		fire(cpu, irq, HIGHEST_PRIO);
	} else {
		cpu.irq.active[irq] = true;
		refresh_irqs(cpu);
	}
}

uint8_t IRQ::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2020: return 0
			| (cpu.irq.priority[0] << 6)
			| (cpu.irq.priority[1] << 4)
			| (cpu.irq.priority[2] << 2)
			| (cpu.irq.priority[3] << 0)
			;

		case 0x2021: return 0
			| (cpu.irq.priority[4] << 6)
			| (cpu.irq.priority[5] << 4)
			| (cpu.irq.priority[6] << 2)
			| (cpu.irq.priority[7] << 0)
			;

		case 0x2022: return 0
			| (cpu.irq.priority[8] << 0)
			;

		case 0x2023: return 0
			| (cpu.irq.enable[0x03] ? 0b10000000 : 0)
			| (cpu.irq.enable[0x04] ? 0b01000000 : 0)
			| (cpu.irq.enable[0x05] ? 0b00100000 : 0)
			| (cpu.irq.enable[0x06] ? 0b00010000 : 0)
			| (cpu.irq.enable[0x07] ? 0b00001000 : 0)
			| (cpu.irq.enable[0x08] ? 0b00000100 : 0)
			| (cpu.irq.enable[0x09] ? 0b00000010 : 0)
			| (cpu.irq.enable[0x0A] ? 0b00000001 : 0)
			;

		case 0x2024: return 0
			| (cpu.irq.enable[0x0B] ? 0b00100000 : 0)
			| (cpu.irq.enable[0x0C] ? 0b00010000 : 0)
			| (cpu.irq.enable[0x0D] ? 0b00001000 : 0)
			| (cpu.irq.enable[0x0E] ? 0b00000100 : 0)
			| (cpu.irq.enable[0x13] ? 0b00000010 : 0)
			| (cpu.irq.enable[0x14] ? 0b00000001 : 0)
			;

		case 0x2025: return 0 
			| (cpu.irq.enable[0x15] ? 0b10000000 : 0)
			| (cpu.irq.enable[0x16] ? 0b01000000 : 0)
			| (cpu.irq.enable[0x17] ? 0b00100000 : 0)
			| (cpu.irq.enable[0x18] ? 0b00010000 : 0)
			| (cpu.irq.enable[0x19] ? 0b00001000 : 0)
			| (cpu.irq.enable[0x1A] ? 0b00000100 : 0)
			| (cpu.irq.enable[0x1B] ? 0b00000010 : 0)
			| (cpu.irq.enable[0x1C] ? 0b00000001 : 0)
			;

		case 0x2026: return 0
			| (cpu.irq.enable[0x0F] ? 0b10000000 : 0)
			| (cpu.irq.enable[0x10] ? 0b01000000 : 0)
			| (cpu.irq.enable[0x11] ? 0b00100000 : 0)
			| (cpu.irq.enable[0x12] ? 0b00010000 : 0)
			| (cpu.irq.enable[0x1D] ? 0b00000100 : 0)
			| (cpu.irq.enable[0x1E] ? 0b00000010 : 0)
			| (cpu.irq.enable[0x1F] ? 0b00000001 : 0)
			;

		case 0x2027: return 0
			| (cpu.irq.active[0x03] ? 0b10000000 : 0)
			| (cpu.irq.active[0x04] ? 0b01000000 : 0)
			| (cpu.irq.active[0x05] ? 0b00100000 : 0)
			| (cpu.irq.active[0x06] ? 0b00010000 : 0)
			| (cpu.irq.active[0x07] ? 0b00001000 : 0)
			| (cpu.irq.active[0x08] ? 0b00000100 : 0)
			| (cpu.irq.active[0x09] ? 0b00000010 : 0)
			| (cpu.irq.active[0x0A] ? 0b00000001 : 0)
			;

		case 0x2028: return 0
			| (cpu.irq.active[0x0B] ? 0b00100000 : 0)
			| (cpu.irq.active[0x0C] ? 0b00010000 : 0)
			| (cpu.irq.active[0x0D] ? 0b00001000 : 0)
			| (cpu.irq.active[0x0E] ? 0b00000100 : 0)
			| (cpu.irq.active[0x13] ? 0b00000010 : 0)
			| (cpu.irq.active[0x14] ? 0b00000001 : 0)
			;

		case 0x2029: return 0 
			| (cpu.irq.active[0x15] ? 0b10000000 : 0)
			| (cpu.irq.active[0x16] ? 0b01000000 : 0)
			| (cpu.irq.active[0x17] ? 0b00100000 : 0)
			| (cpu.irq.active[0x18] ? 0b00010000 : 0)
			| (cpu.irq.active[0x19] ? 0b00001000 : 0)
			| (cpu.irq.active[0x1A] ? 0b00000100 : 0)
			| (cpu.irq.active[0x1B] ? 0b00000010 : 0)
			| (cpu.irq.active[0x1C] ? 0b00000001 : 0)
			;

		case 0x202A: return 0
			| (cpu.irq.active[0x0F] ? 0b10000000 : 0)
			| (cpu.irq.active[0x10] ? 0b01000000 : 0)
			| (cpu.irq.active[0x11] ? 0b00100000 : 0)
			| (cpu.irq.active[0x12] ? 0b00010000 : 0)
			| (cpu.irq.active[0x1D] ? 0b00000100 : 0)
			| (cpu.irq.active[0x1E] ? 0b00000010 : 0)
			| (cpu.irq.active[0x1F] ? 0b00000001 : 0)
			;
	}

	return 0xFF;
}

void IRQ::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	switch (address) {
		case 0x2020: 
			cpu.irq.priority[0] = (data >> 6) & 3;
			cpu.irq.priority[1] = (data >> 4) & 3;
			cpu.irq.priority[2] = (data >> 2) & 3;
			cpu.irq.priority[3] = (data >> 0) & 3;
			break ;

		case 0x2021:
			cpu.irq.priority[4] = (data >> 6) & 3;
			cpu.irq.priority[5] = (data >> 4) & 3;
			cpu.irq.priority[6] = (data >> 2) & 3;
			cpu.irq.priority[7] = (data >> 0) & 3;
			break ;

		case 0x2022:
			cpu.irq.priority[8] = (data >> 0) & 3;
			break ;

		case 0x2023: 
			cpu.irq.enable[0x03] = data & 0b10000000;
			cpu.irq.enable[0x04] = data & 0b01000000;
			cpu.irq.enable[0x05] = data & 0b00100000;
			cpu.irq.enable[0x06] = data & 0b00010000;
			cpu.irq.enable[0x07] = data & 0b00001000;
			cpu.irq.enable[0x08] = data & 0b00000100;
			cpu.irq.enable[0x09] = data & 0b00000010;
			cpu.irq.enable[0x0A] = data & 0b00000001;
			break ;

		case 0x2024: 
			cpu.irq.enable[0x0B] = data & 0b00100000;
			cpu.irq.enable[0x0C] = data & 0b00010000;
			cpu.irq.enable[0x0D] = data & 0b00001000;
			cpu.irq.enable[0x0E] = data & 0b00000100;
			cpu.irq.enable[0x13] = data & 0b00000010;
			cpu.irq.enable[0x14] = data & 0b00000001;
			break ;

		case 0x2025: 
			cpu.irq.enable[0x15] = data & 0b10000000;
			cpu.irq.enable[0x16] = data & 0b01000000;
			cpu.irq.enable[0x17] = data & 0b00100000;
			cpu.irq.enable[0x18] = data & 0b00010000;
			cpu.irq.enable[0x19] = data & 0b00001000;
			cpu.irq.enable[0x1A] = data & 0b00000100;
			cpu.irq.enable[0x1B] = data & 0b00000010;
			cpu.irq.enable[0x1C] = data & 0b00000001;
			break ;

		case 0x2026:
			cpu.irq.enable[0x0F] = data & 0b10000000;
			cpu.irq.enable[0x10] = data & 0b01000000;
			cpu.irq.enable[0x11] = data & 0b00100000;
			cpu.irq.enable[0x12] = data & 0b00010000;
			cpu.irq.enable[0x1D] = data & 0b00000100;
			cpu.irq.enable[0x1E] = data & 0b00000010;
			cpu.irq.enable[0x1F] = data & 0b00000001;
			break ;

		case 0x2027: 
			if (data & 0b10000000) cpu.irq.active[0x03] = false;
			if (data & 0b01000000) cpu.irq.active[0x04] = false;
			if (data & 0b00100000) cpu.irq.active[0x05] = false;
			if (data & 0b00010000) cpu.irq.active[0x06] = false;
			if (data & 0b00001000) cpu.irq.active[0x07] = false;
			if (data & 0b00000100) cpu.irq.active[0x08] = false;
			if (data & 0b00000010) cpu.irq.active[0x09] = false;
			if (data & 0b00000001) cpu.irq.active[0x0A] = false;
			break ;

		case 0x2028: 
			if (data & 0b00100000) cpu.irq.active[0x0B] = false;
			if (data & 0b00010000) cpu.irq.active[0x0C] = false;
			if (data & 0b00001000) cpu.irq.active[0x0D] = false;
			if (data & 0b00000100) cpu.irq.active[0x0E] = false;
			if (data & 0b00000010) cpu.irq.active[0x13] = false;
			if (data & 0b00000001) cpu.irq.active[0x14] = false;
			break ;

		case 0x2029: 
			if (data & 0b10000000) cpu.irq.active[0x15] = false;
			if (data & 0b01000000) cpu.irq.active[0x16] = false;
			if (data & 0b00100000) cpu.irq.active[0x17] = false;
			if (data & 0b00010000) cpu.irq.active[0x18] = false;
			if (data & 0b00001000) cpu.irq.active[0x19] = false;
			if (data & 0b00000100) cpu.irq.active[0x1A] = false;
			if (data & 0b00000010) cpu.irq.active[0x1B] = false;
			if (data & 0b00000001) cpu.irq.active[0x1C] = false;
			break ;

		case 0x202A:
			if (data & 0b10000000) cpu.irq.active[0x0F] = false;
			if (data & 0b01000000) cpu.irq.active[0x10] = false;
			if (data & 0b00100000) cpu.irq.active[0x11] = false;
			if (data & 0b00010000) cpu.irq.active[0x12] = false;
			if (data & 0b00000100) cpu.irq.active[0x1D] = false;
			if (data & 0b00000010) cpu.irq.active[0x1E] = false;
			if (data & 0b00000001) cpu.irq.active[0x1F] = false;
			break ;
	}

	refresh_irqs(cpu);
}
