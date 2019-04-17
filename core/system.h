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

uint8_t cpu_read8(ProcessorState& cpu, uint32_t address);
uint8_t cpu_write8(ProcessorState& cpu, uint8_t data, uint32_t address);

void step_cpu(ProcessorState& cpu);
