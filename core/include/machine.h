#pragma once

#include <stdint.h>

#include "lcd.h"
#include "irq.h"
#include "tim256.h"
#include "rtc.h"
#include "blitter.h"
#include "control.h"

#define EXPORT   __attribute__((visibility ("default"))) extern "C"
#define EXPORTED extern "C"
#define IMPORTED extern "C"

union CPUState {
	struct {
		uint8_t a;
		uint8_t b;
		uint8_t l;
		uint8_t h;
	};

	struct  __attribute__((packed)) {
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
		};
	};
};

struct MachineState {
	CPUState reg;
	IRQState irq;
	LCDState lcd;
	RTCState rtc;
	ControlState ctrl;
	TIM256State tim256;

	uint8_t bus_cap;
	int clocks;
	int osc3_overflow;
	bool sleeping;
	bool halted;

	uint8_t ram[0x1000];
};

// Library functions
IMPORTED uint8_t cpu_read_cart(MachineState& cpu, uint32_t address);
IMPORTED void cpu_write_cart(MachineState& cpu, uint8_t data, uint32_t address);

EXPORTED uint8_t cpu_read(MachineState& cpu, uint32_t address);
EXPORTED void cpu_write(MachineState& cpu, uint8_t data, uint32_t address);

EXPORTED void cpu_step(MachineState& cpu);
EXPORTED bool cpu_advance(MachineState& cpu, int ticks);

// Clock management
void cpu_clock(MachineState& cpu, int cycles);

// These are memory access helpers
uint8_t cpu_read8(MachineState& cpu, uint32_t address);
void cpu_write8(MachineState& cpu, uint8_t data, uint32_t address);
uint16_t cpu_read16(MachineState& cpu, uint32_t address);
void cpu_write16(MachineState& cpu, uint16_t data, uint32_t address);
uint8_t cpu_imm8(MachineState& cpu);
uint16_t cpu_imm16(MachineState& cpu);
void cpu_push8(MachineState& cpu, uint8_t t);
uint8_t cpu_pop8(MachineState& cpu);
void cpu_push16(MachineState& cpu, uint16_t t);
uint16_t cpu_pop16(MachineState& cpu);
int inst_advance(MachineState& cpu);
