#include <string.h>
#include "machine.h"

static const uint8_t TIMER_MASK[] = {
	0b10001111,
	0b00001111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b00000000,
	0b00000000
};

int PRESCALE_OSC1[] = {
	1, 2, 4, 8, 16, 32, 64, 128
};

int PRESCALE_OSC3[] = {
	2, 8, 32, 64, 128, 256, 1024, 4096
};

void Timers::reset(Machine::State& cpu) {
	memset(&cpu.timers, 0, sizeof(cpu.timers));
}

void Timers::clock(Machine::State& cpu, int osc1, int osc3) {
}

uint8_t Timers::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
	case 0x2018:
		return 0
			| (cpu.timers.timer[0].lo_clock_ratio)
			| (cpu.timers.timer[0].lo_clock_ctrl ? 0b00001000 : 0)
			| (cpu.timers.timer[0].hi_clock_ratio << 4)
			| (cpu.timers.timer[0].hi_clock_ctrl ? 0b10000000 : 0);

	case 0x201A:
		return 0
			| (cpu.timers.timer[1].lo_clock_ratio)
			| (cpu.timers.timer[1].lo_clock_ctrl ? 0b00001000 : 0)
			| (cpu.timers.timer[1].hi_clock_ratio << 4)
			| (cpu.timers.timer[1].hi_clock_ctrl ? 0b10000000 : 0);

	case 0x201C:
		return 0
			| (cpu.timers.timer[2].lo_clock_ratio)
			| (cpu.timers.timer[2].lo_clock_ctrl ? 0b00001000 : 0)
			| (cpu.timers.timer[2].hi_clock_ratio << 4)
			| (cpu.timers.timer[2].hi_clock_ctrl ? 0b10000000 : 0);

	case 0x2019:
		return 0	
			| (cpu.timers.osc3_enable ? 0b00100000 : 0)
			| (cpu.timers.osc1_enable ? 0b00010000 : 0)

			| (cpu.timers.timer[0].lo_clock_source ? 0b01 : 0)
			| (cpu.timers.timer[0].hi_clock_source ? 0b10 : 0);
	case 0x201B:
		return 0
			| (cpu.timers.timer[1].lo_clock_source ? 0b01 : 0)
			| (cpu.timers.timer[1].hi_clock_source ? 0b10 : 0);
	case 0x201D:
		return 0
			| (cpu.timers.timer[2].lo_clock_source ? 0b01 : 0)
			| (cpu.timers.timer[2].hi_clock_source ? 0b10 : 0);

	// Timer 0/1
	case 0x2030 ... 0x2037:
		return cpu.timers.timer[0].bytes[address & 0b111];

	// Timer 2/3
	case 0x2038 ... 0x203F:
		return cpu.timers.timer[1].bytes[address & 0b111];

	// Timer 4/5
	case 0x2048 ... 0x204F:
		return cpu.timers.timer[2].bytes[address & 0b111];
	
	default:
		return 0xCD;
	}
}

static inline void setup_timer(Timers::Timer& timer) {
	if (timer.mode16) {
		timer.hi_input = 0;
		timer.hi_preset = 0;
		timer.hi_running = 0;

		if (timer.lo_preset) {
			timer.count = timer.preset;
			timer.lo_preset = 0;
		}
	} else {
		if (timer.lo_preset) {
			timer.count_bytes[0] = timer.preset_bytes[0];
			timer.lo_preset = 0;
		}

		if (timer.hi_preset) {
			timer.count_bytes[1] = timer.preset_bytes[1];
			timer.hi_preset = 0;
		}
	}
}

void Timers::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	switch (address) {
	case 0x2018:
		cpu.timers.timer[0].lo_clock_ratio = data & 0b0111;
		cpu.timers.timer[0].lo_clock_ctrl = (data & 0b1000) ? true : false;
		cpu.timers.timer[0].hi_clock_ratio = (data >> 4) & 0b0111;
		cpu.timers.timer[0].hi_clock_ctrl = (data & 0b10000000) ? true : false;
		break ;
	case 0x201A:
		cpu.timers.timer[1].lo_clock_ratio = data & 0b0111;
		cpu.timers.timer[1].lo_clock_ctrl = (data & 0b1000) ? true : false;
		cpu.timers.timer[1].hi_clock_ratio = (data >> 4) & 0b0111;
		cpu.timers.timer[1].hi_clock_ctrl = (data & 0b10000000) ? true : false;
		break ;
	case 0x201C:
		cpu.timers.timer[2].lo_clock_ratio = data & 0b0111;
		cpu.timers.timer[2].lo_clock_ctrl = (data & 0b1000) ? true : false;
		cpu.timers.timer[2].hi_clock_ratio = (data >> 4) & 0b0111;
		cpu.timers.timer[2].hi_clock_ctrl = (data & 0b10000000) ? true : false;
		break ;

	case 0x2019:
		cpu.timers.osc3_enable = (data & 0b00100000) ? true : false;
		cpu.timers.osc1_enable = (data & 0b00010000) ? true : false;

		cpu.timers.timer[0].lo_clock_source = (data & 0b01) ? true : false;
		cpu.timers.timer[0].hi_clock_source = (data & 0b10) ? true : false;
		break ;
	case 0x201B:
		cpu.timers.timer[1].lo_clock_source = (data & 0b01) ? true : false;
		cpu.timers.timer[1].hi_clock_source = (data & 0b10) ? true : false;
		break ;
	case 0x201D:
		cpu.timers.timer[2].lo_clock_source = (data & 0b01) ? true : false;
		cpu.timers.timer[2].hi_clock_source = (data & 0b10) ? true : false;
		break ;

	// Timer 0/1
	case 0x2030 ... 0x2031:
		cpu.timers.timer[0].bytes[address & 0b111] = data & TIMER_MASK[address & 0b111];
		setup_timer(cpu.timers.timer[0]);
		break ;
	case 0x2032 ... 0x2035:
		cpu.timers.timer[0].bytes[address & 0b111] = data & TIMER_MASK[address & 0b111];
		break ;

	// Timer 2/3
	case 0x2038 ... 0x2039:
		cpu.timers.timer[1].bytes[address & 0b111] = data & TIMER_MASK[address & 0b111];
		setup_timer(cpu.timers.timer[1]);
		break ;
	case 0x203A ... 0x203D:
		cpu.timers.timer[1].bytes[address & 0b111] = data & TIMER_MASK[address & 0b111];
		break ;

	// Timer 4/5
	case 0x2048 ... 0x2049:
		cpu.timers.timer[2].bytes[address & 0b111] = data & TIMER_MASK[address & 0b111];
		setup_timer(cpu.timers.timer[2]);
		break ;
	case 0x204A ... 0x204D:
		cpu.timers.timer[2].bytes[address & 0b111] = data & TIMER_MASK[address & 0b111];
		break ;
	}
}
