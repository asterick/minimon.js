#include <stdint.h>

#include "machine.h"
#include "debug.h"

uint8_t lcd_read_reg(MachineState& cpu, uint32_t address) {
	dprintf("LCD READ %x", address);

	return 0xCD;
}

void lcd_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	dprintf("LCD WRITE %x %x", address, data);
}
