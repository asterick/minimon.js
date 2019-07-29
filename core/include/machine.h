#pragma once

#include <stdint.h>

#include "lcd.h"
#include "irq.h"
#include "tim256.h"
#include "timers.h"
#include "rtc.h"
#include "blitter.h"
#include "control.h"

namespace CPU {
	union State {
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
}

namespace Machine {
	struct State {
		CPU::State reg;
		IRQ::State irq;
		LCD::State lcd;
		RTC::State rtc;
		Control::State ctrl;
		TIM256::State tim256;
		Blitter::State blitter;
		Timers::State timers;

		uint8_t bus_cap;
		int clocks;
		int osc1_overflow;
		bool sleeping;
		bool halted;

		union {
			uint8_t ram[0x1000];
		 	Blitter::Overlay overlay;
		};
	};
}

// External defines
extern "C" uint8_t cpu_read_cart(Machine::State& cpu, uint32_t address);
extern "C" void cpu_write_cart(Machine::State& cpu, uint8_t data, uint32_t address);

// Library functions
extern "C" Machine::State* const get_machine();
extern "C" uint8_t cpu_read_cart(Machine::State& cpu, uint32_t address);
extern "C" void cpu_write_cart(Machine::State& cpu, uint8_t data, uint32_t address);

extern "C" uint8_t cpu_read(Machine::State& cpu, uint32_t address);
extern "C" void cpu_write(Machine::State& cpu, uint8_t data, uint32_t address);

extern "C" void cpu_step(Machine::State& cpu);
extern "C" bool cpu_advance(Machine::State& cpu, int ticks);

// Clock management
void cpu_clock(Machine::State& cpu, int cycles);

// These are memory access helpers
uint8_t cpu_read8(Machine::State& cpu, uint32_t address);
void cpu_write8(Machine::State& cpu, uint8_t data, uint32_t address);
uint16_t cpu_read16(Machine::State& cpu, uint32_t address);
void cpu_write16(Machine::State& cpu, uint16_t data, uint32_t address);
uint8_t cpu_imm8(Machine::State& cpu);
uint16_t cpu_imm16(Machine::State& cpu);
void cpu_push8(Machine::State& cpu, uint8_t t);
uint8_t cpu_pop8(Machine::State& cpu);
void cpu_push16(Machine::State& cpu, uint16_t t);
uint16_t cpu_pop16(Machine::State& cpu);
int inst_advance(Machine::State& cpu);
