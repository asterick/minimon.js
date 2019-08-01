/*
ISC License

Copyright (c) 2019, Bryon Vandiver

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <string.h>
#include "machine.h"

struct TimerIRQ {
	int lo_underflow;
	int hi_underflow;
	int lo_compare;
};

static const TimerIRQ irqs[] = {
	{ IRQ::IRQ_TIM0, IRQ::IRQ_TIM1,                -1 },
	{ IRQ::IRQ_TIM2, IRQ::IRQ_TIM3,                -1 },
	{            -1, IRQ::IRQ_TIM5, IRQ::IRQ_TIM5_CMP } 
};

static const uint8_t TIMER_MASK[] = {
	0b10001111,
	0b00001111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111
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

static inline void irq(Machine::State& cpu, int vec) {
	if (vec < 0) return ;

	IRQ::trigger(cpu, (IRQ::Vector) vec);
}

static inline int ticks(Timers::State& timers, bool clock_source, bool clock_ctrl, int clock_ratio, int osc1, int osc3) {
	if (!clock_ctrl) return 0;

	if (clock_source) {
		int adjust = PRESCALE_OSC1[clock_ratio];
		return (timers.osc1_prescale % adjust + osc1) / adjust;
	} else {
		int adjust = PRESCALE_OSC3[clock_ratio];
		return (timers.osc3_prescale % adjust + osc3) / adjust;
	}
}

void Timers::clock(Machine::State& cpu, int osc1, int osc3) {
	if (!cpu.timers.osc1_enable) osc1 = 0;
	if (!cpu.timers.osc3_enable) osc3 = 0;

	for (int i = 0; i < 3; i++) {
		Timer& timer = cpu.timers.timer[i];
		const TimerIRQ& vects = irqs[i];

		if (timer.mode16) {
			if (!timer.lo_running) continue ;

			int count = timer.count - ticks(cpu.timers, timer.lo_clock_source, timer.lo_clock_ctrl, timer.lo_clock_ratio, osc1, osc3);
			
			if (count < 0) {
				irq(cpu, vects.hi_underflow);
				do {
					count += timer.preset + 1;	
				} while (count < 0);
			}

			timer.count = count;
		} else {
			if (timer.lo_running) {
				int count = timer.count_bytes[0] - ticks(cpu.timers, timer.lo_clock_source, timer.lo_clock_ctrl, timer.lo_clock_ratio, osc1, osc3);

				if (count < 0) {
					irq(cpu, vects.lo_underflow);
					do {
						count += timer.preset_bytes[0] + 1;
					} while (count < 0);
				}
				timer.count_bytes[0] = count;
			}

			if (timer.hi_running) {
				int count = timer.count_bytes[1] - ticks(cpu.timers, timer.hi_clock_source, timer.hi_clock_ctrl, timer.hi_clock_ratio, osc1, osc3);

				if (count < 0) {
					irq(cpu, vects.hi_underflow);
					do {
						count += timer.preset_bytes[1] + 1;	
					} while (count < 0);
				}
				timer.count_bytes[1] = count;
			}
		}
	}

	cpu.timers.osc1_prescale += osc1;
	cpu.timers.osc3_prescale += osc3;
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
