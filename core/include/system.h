#pragma once

#include <stdint.h>

union ProcessorRegisters {
	struct {
		uint8_t a;
		uint8_t b;
		uint8_t l;
		uint8_t h;
	};

	struct {
		uint16_t ba;
		uint16_t hl;
		uint16_t ix;
		uint16_t iy;
		uint16_t pc;
		uint16_t sp;

		uint8_t br;
		uint8_t ep;
		uint8_t xp;
		uint8_t yp;

		uint8_t cb;
		uint8_t nb;

		union {
			uint8_t sc;

			struct {
				unsigned int z:1;
				unsigned int c:1;
				unsigned int v:1;
				unsigned int n:1;
				unsigned int d:1;
				unsigned int u:1;
				unsigned int i:2;

				unsigned int f0:1;
				unsigned int f1:1;
				unsigned int f2:1;
				unsigned int f3:1;
			} flag;
		} ;
	};
};

struct ProcessorState {
	 ProcessorRegisters reg;
};

enum InterruptVector : uint8_t {
	IRQ_RESET,
	IRQ_DIV_ZERO,
	IRQ_WATCHDOG
};

void cpu_int(ProcessorState& cpu, InterruptVector irq, int priority = 0);

uint8_t cpu_read8(ProcessorState& cpu, uint32_t address);
uint16_t cpu_read16(ProcessorState& cpu, uint32_t address);
void cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address);
void cpu_write16(ProcessorState& cpu, uint16_t data, uint32_t address);


uint8_t cpu_imm8(ProcessorState& cpu);
uint16_t cpu_imm16(ProcessorState& cpu);
void cpu_push8(ProcessorState& cpu, uint8_t t);
uint8_t cpu_pop8(ProcessorState& cpu);
void cpu_push16(ProcessorState& cpu, uint16_t t);
uint16_t cpu_pop16(ProcessorState& cpu);

void cpu_step(ProcessorState& cpu);
