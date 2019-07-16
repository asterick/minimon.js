#pragma once

enum InterruptVector : uint8_t {
	IRQ_RESET,
	IRQ_DIV_ZERO,
	IRQ_WATCHDOG
};

static const int IRC_PRIO_HIGHEST = 3;

uint8_t irq_read_reg(ProcessorState& cpu, uint32_t address);
void irq_write_reg(ProcessorState& cpu, uint8_t data, uint32_t address);
void irq_trigger(InterruptVector irq);

void cpu_interrupt(ProcessorState& cpu, InterruptVector irq, int level);
