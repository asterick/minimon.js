#pragma once

enum InterruptVector : uint8_t {
	IRQ_RESET,
	IRQ_DIV_ZERO,
	IRQ_WATCHDOG
};

static const int IRC_PRIO_HIGHEST = 3;

extern "C" {
	void cpu_interrupt(ProcessorState& cpu, InterruptVector irq, int level);
}
