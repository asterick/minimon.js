#pragma once

struct MachineState;

enum InterruptVector : uint8_t {
	IRQ_RESET,
	IRQ_DIV_ZERO,
	IRQ_WATCHDOG
};

struct IRQState {
	uint32_t priority;
	uint32_t enable;
	uint32_t active;
};

static const int IRC_PRIO_HIGHEST = 3;

uint8_t irq_read_reg(MachineState& cpu, uint32_t address);
void irq_write_reg(MachineState& cpu, uint8_t data, uint32_t address);
void irq_trigger(InterruptVector irq);

void cpu_interrupt(MachineState& cpu, InterruptVector irq, int level);
