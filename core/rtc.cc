#include <stdint.h>

#include "machine.h"

void RTC::reset(Machine::State& cpu) {
	cpu.rtc.running = false;
	cpu.rtc.value = 0;
}

void RTC::clock(Machine::State& cpu, int osc1) {
	if (cpu.rtc.running) {
		cpu.rtc.value += osc1;
	}
}

uint8_t RTC::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2008: return cpu.rtc.running ? 0b1 :0b0;
		case 0x2009: return cpu.rtc.value >> 15;
		case 0x200A: return cpu.rtc.value >> 23;
		case 0x200B: return cpu.rtc.value >> 31;
	}

	return 0;
}

void RTC::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	if (address == 0x2008) {
		cpu.rtc.running = (data & 0b01);

		if (data & 0b10) {
			cpu.rtc.value = 0;
		}
	}
}
