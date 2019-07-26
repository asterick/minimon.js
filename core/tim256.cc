#include <stdint.h>

#include "machine.h"

// This is the fractional part before an overflow of the clock timer
static const int FRACT_32Hz  = 0b0000011111111111;
static const int FRACT_8Hz   = 0b0001111111111111;
static const int FRACT_2Hz   = 0b0111111111111111;
static const int FRACT_1Hz   = 0b1111111111111111;

void TIM256::reset(Machine::State& cpu) {
	cpu.tim256.running = false;
	cpu.tim256.value = 0;
}

static inline void trigger_overflow(Machine::State& cpu, int mask, IRQ::Vector vec, int before, int ticks) {
	if ((before & mask) + ticks > mask) {
		IRQ::trigger(cpu, vec);
	}
}

void TIM256::clock(Machine::State& cpu, int osc1) {
	if (!cpu.tim256.running) return ;

	trigger_overflow(cpu, FRACT_32Hz, IRQ::IRQ_32HZ, cpu.tim256.value, osc1);
	trigger_overflow(cpu,  FRACT_8Hz,  IRQ::IRQ_8HZ, cpu.tim256.value, osc1);
	trigger_overflow(cpu,  FRACT_2Hz,  IRQ::IRQ_2HZ, cpu.tim256.value, osc1);
	trigger_overflow(cpu,  FRACT_1Hz,  IRQ::IRQ_1HZ, cpu.tim256.value, osc1);

	cpu.tim256.value += osc1;
}

uint8_t TIM256::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
	case 0x2040: return cpu.tim256.running ? 0b1 : 0b0;
	case 0x2041: return cpu.tim256.value >> 8;
	}

	return 0;
}

void TIM256::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	if (address == 0x2040) {
		cpu.tim256.running = (data & 0b01);

		if (data & 0b10) {
			cpu.tim256.value = 0;
		}
	}
}
