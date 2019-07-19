#include <stdint.h>

#include "machine.h"

// This is the fractional part before an overflow of the clock timer
static const int FRACT_32Hz  = 0b0000011111111111;
static const int FRACT_8Hz   = 0b0001111111111111;
static const int FRACT_2Hz   = 0b0111111111111111;
static const int FRACT_1Hz   = 0b1111111111111111;

void tim256_reset(MachineState& cpu) {
	cpu.tim256.running = false;
	cpu.tim256.value = 0;
}

static inline void trigger_overflow(MachineState& cpu, int mask, InterruptVector vec, int before, int ticks) {
	if ((before & mask) + ticks > mask) {
		irq_trigger(cpu, vec);
	}
}

void tim256_clock(MachineState& cpu, int osc3) {
	if (!cpu.tim256.running) return ;

	trigger_overflow(cpu, FRACT_32Hz, IRQ_32HZ, cpu.tim256.value, osc3);
	trigger_overflow(cpu,  FRACT_8Hz,  IRQ_8HZ, cpu.tim256.value, osc3);
	trigger_overflow(cpu,  FRACT_2Hz,  IRQ_2HZ, cpu.tim256.value, osc3);
	trigger_overflow(cpu,  FRACT_1Hz,  IRQ_1HZ, cpu.tim256.value, osc3);

	cpu.tim256.value += osc3;
}

uint8_t tim256_read_reg(MachineState& cpu, uint32_t address) {
	switch (address) {
	case 0x2040: return cpu.tim256.running ? 0b1 : 0b0;
	case 0x2041: return cpu.tim256.value >> 8;
	}

	return 0;
}

void tim256_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	if (address == 0x2040) {
		cpu.tim256.running = (data & 0b01);

		if (data & 0b10) {
			cpu.tim256.value = 0;
		}
	}
}
