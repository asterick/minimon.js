#include <stdint.h>

#include "machine.h"

void rtc_reset(MachineState& cpu) {
	cpu.rtc.running = false;
	cpu.rtc.value = 0;
}

void rtc_clock(MachineState& cpu, int osc3) {
	if (cpu.rtc.running) {
		cpu.rtc.value ++;
	}
}

uint8_t rtc_read_reg(MachineState& cpu, uint32_t address) {
	switch (address) {
		case 0x2008: return cpu.rtc.running ? 0b1 :0b0;
		case 0x2009: return cpu.rtc.value >> 15;
		case 0x200A: return cpu.rtc.value >> 23;
		case 0x200B: return cpu.rtc.value >> 31;
	}

	return 0;
}

void rtc_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	if (address == 0x2008) {
		cpu.rtc.running = (data & 0b01);

		if (data & 0b10) {
			cpu.rtc.value = 0;
		}
	}
}
