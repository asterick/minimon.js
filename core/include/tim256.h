#pragma once

void tim256_reset(MachineState& cpu);
uint8_t tim256_read(MachineState& cpu, uint32_t address);
void tim256_write(MachineState& cpu, uint8_t data, uint32_t address);
