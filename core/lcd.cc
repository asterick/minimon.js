#include <stdint.h>

#include "machine.h"
#include "lcd.h"

uint8_t lcd_read_reg(ProcessorState& cpu, uint32_t address) {
	return 0xCD;
}

void lcd_write_reg(ProcessorState& cpu, uint8_t data, uint32_t address) {
}
