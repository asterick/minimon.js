#pragma once

struct RTCState {
	bool running;
	uint64_t value;
};

void rtc_reset(MachineState& cpu);
void rtc_clock(MachineState& cpu, int osc3);
uint8_t rtc_read_reg(MachineState& cpu, uint32_t address);
void rtc_write_reg(MachineState& cpu, uint8_t data, uint32_t address);
