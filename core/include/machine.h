#pragma once

enum InterruptVector : uint8_t {
	IRQ_RESET,
	IRQ_DIV_ZERO,
	IRQ_WATCHDOG
};

extern "C" {
	void cpu_interrupt(ProcessorState& cpu, InterruptVector irq);
	uint8_t cpu_read8(ProcessorState& cpu, uint32_t address);
	void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address);
	void cpu_step(ProcessorState& cpu);

	// These are memory access helpers
	uint16_t cpu_read16(ProcessorState& cpu, uint32_t address);
	void cpu_write16(ProcessorState& cpu, uint16_t data, uint32_t address);
	uint8_t cpu_imm8(ProcessorState& cpu);
	uint16_t cpu_imm16(ProcessorState& cpu);
	void cpu_push8(ProcessorState& cpu, uint8_t t);
	uint8_t cpu_pop8(ProcessorState& cpu);
	void cpu_push16(ProcessorState& cpu, uint16_t t);
	uint16_t cpu_pop16(ProcessorState& cpu);
}
