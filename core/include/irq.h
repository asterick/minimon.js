#pragma once

struct MachineState;

enum InterruptVector : uint8_t {
	IRQ_RESET,
	IRQ_DIV_ZERO,
	IRQ_WATCHDOG,
	IRQ_BLT_COPY,
	IRQ_BLT_OVERFLOW,
	IRQ_TIM3,
	IRQ_TIM2,
	IRQ_TIM1,
	IRQ_TIM0,
	IRQ_TIM5,
	IRQ_TIM4,
	IRQ_TIM5_CMP,
	IRQ_32HZ,
	IRQ_8HZ,
	IRQ_2HZ,
	IRQ_1HZ,
	IRQ_IR_RCV,
	IRQ_SHOCK,
	IRQ_UNKNOWN1,
	IRQ_UNKNOWN2,
	IRQ_K09,
	IRQ_K08,
	IRQ_K07,
	IRQ_K06,
	IRQ_K05,
	IRQ_K04,
	IRQ_K03,
	IRQ_K02,
	IRQ_K01,
	IRQ_K00,
	IRQ_UNKNOWN3,
	IRQ_UNKNOWN4,
	IRQ_UNKNOWN5,

	TOTAL_HARDWARE_IRQS
};

struct IRQState {
	union {
		uint32_t priority;
		uint8_t priority_bytes[3];
	};
	union {
		uint32_t enable;
		uint8_t enable_bytes[4];
	};
	union {
		uint32_t active;
		uint8_t active_bytes[4];
	};

	int next_priority;
	InterruptVector next_irq;
	bool block;
};

static const int IRQ_PRIO_HIGHEST = 3;
static const int FIRST_MASKABLE_IRQ = 3;

void irq_reset(MachineState& cpu);
void irq_fire(MachineState& cpu);
uint8_t irq_read_reg(MachineState& cpu, uint32_t address);
void irq_write_reg(MachineState& cpu, uint8_t data, uint32_t address);
void irq_trigger(MachineState& cpu, InterruptVector irq);
