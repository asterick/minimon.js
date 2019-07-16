#include <stdint.h>

#include "machine.h"

struct IRQVectorTable {
	bool maskable;
	int priority_group;
	int bit_group;
};

static const IRQVectorTable IRQ_TABLE[TOTAL_HARDWARE_IRQS] = {
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
	0b11110111,

	0b11111111,
	0b00111111,
	0b11111111,
	0b11110111,
};

static void refresh_irqs(MachineState& cpu) {
	cpu.irq.next_priority = 0;

	for (int irq = FIRST_MASKABLE_IRQ; irq < TOTAL_HARDWARE_IRQS; irq++) {
		const IRQVectorTable& info = IRQ_TABLE[irq];

		// Is IRQ active and enabled?
		if (cpu.irq.active & cpu.irq.enable & (1 << info.bit_group)) {
			continue ;
		}

		int priority = (cpu.irq.priority >> info.priority_group) & IRQ_PRIO_HIGHEST;

		if (cpu.irq.next_priority < priority) {
			cpu.irq.next_priority = priority;
			cpu.irq.next_irq = (InterruptVector) irq;
		}
	}
}

void irq_reset(MachineState& cpu) {
	cpu.irq.priority = 0;
	cpu.irq.enable = 0;
	cpu.irq.active = 0;

	refresh_irqs(cpu);
}

static void cpu_interrupt(MachineState& cpu, InterruptVector irq, int level) {
	cpu_push8(cpu, cpu.reg.cb);
	cpu_push16(cpu, cpu.reg.pc);
	cpu_push8(cpu, cpu.reg.sc);

	cpu.reg.pc = cpu_read16(cpu, 2 * (int) irq);
	cpu.reg.flag.i = level;
}

void irq_fire(MachineState& cpu) {
	// Cannot interrupt of nb != cb, not sure how this affects divide by zeros
	if (cpu.reg.nb != cpu.reg.cb) return ;

	if (cpu.reg.flag.i < cpu.irq.next_irq) {
		cpu_interrupt(cpu, cpu.irq.next_irq, cpu.irq.next_priority);
	}
}

void irq_trigger(MachineState& cpu, InterruptVector irq) {
	const IRQVectorTable& info = IRQ_TABLE[irq];

	if (info.maskable) {
		cpu.irq.active |= 1 << info.bit_group;

		refresh_irqs(cpu);
	} else {
		// Unmaskable interrupt, fire immediately
		cpu_interrupt(cpu, irq, IRQ_PRIO_HIGHEST);
	}
}

uint8_t irq_read_reg(MachineState& cpu, uint32_t address) {
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

void irq_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	switch (address) {
		case 0x2020: cpu.irq.priority_bytes[0] = data & BIT_MASK[0];
		case 0x2021: cpu.irq.priority_bytes[1] = data & BIT_MASK[1];
		case 0x2022: cpu.irq.priority_bytes[2] = data & BIT_MASK[2];
		case 0x2023: cpu.irq.enable_bytes[0] = data & BIT_MASK[3];
		case 0x2024: cpu.irq.enable_bytes[1] = data & BIT_MASK[4];
		case 0x2025: cpu.irq.enable_bytes[2] = data & BIT_MASK[5];
		case 0x2026: cpu.irq.enable_bytes[3] = data & BIT_MASK[6];
		case 0x2027: cpu.irq.active_bytes[0] &= ~(data & BIT_MASK[7]);
		case 0x2028: cpu.irq.active_bytes[1] &= ~(data & BIT_MASK[8]);
		case 0x2029: cpu.irq.active_bytes[2] &= ~(data & BIT_MASK[9]);
		case 0x202A: cpu.irq.active_bytes[3] &= ~(data & BIT_MASK[10]);
	}

	refresh_irqs(cpu);
}
