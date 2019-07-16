#pragma once

struct MachineState;

uint8_t lcd_read_reg(MachineState& cpu, uint32_t address);
void lcd_write_reg(MachineState& cpu, uint8_t data, uint32_t address);
