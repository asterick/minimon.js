#pragma once

struct TIM256State {
	bool running;
	uint16_t value;
};

void tim256_reset(MachineState& cpu);
void tim256_clock(MachineState& cpu, int osc3);
uint8_t tim256_read_reg(MachineState& cpu, uint32_t address);
void tim256_write_reg(MachineState& cpu, uint8_t data, uint32_t address);
