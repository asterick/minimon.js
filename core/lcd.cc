#include <stdint.h>

#include "machine.h"
#include "debug.h"

void lcd_reset(MachineState& cpu) {
	cpu.lcd.rmw_mode = false;	
}

uint8_t lcd_read_reg(MachineState& cpu, uint32_t address) {
	if (address == 0x20FE) {
		dprintf("READ DISPLAY STATUS");
		return 0;
	} else {
		uint8_t data = cpu.lcd.read_buffer;
		
		data = cpu.lcd.gddram[cpu.lcd.page_address][cpu.lcd.column_address];

		if (cpu.lcd.column_address < 0x83 && !cpu.lcd.rmw_mode) {
			cpu.lcd.column_address++;
		}

		return data;
	}
}

void lcd_write_reg(MachineState& cpu, uint8_t data, uint32_t address) {
	cpu.lcd.read_buffer = data;

	if (cpu.lcd.setting_volume) {
		cpu.lcd.volume = data;
		return;
	}

	if (address == 0x20FE) {
		switch (data) {
		case 0b10101110 ... 0b10101111:
			cpu.lcd.display_enable = data & 1;
			break ;

		case 0b01000000 ... 0b01111111:
			cpu.lcd.start_address = data & 0b111111;
			break ;

		case 0b00000000 ... 0b00001111:
			cpu.lcd.column_address = (cpu.lcd.column_address & 0xF0) | (data & 0xF);

			if (cpu.lcd.column_address > 0x83) cpu.lcd.column_address = 0x83;
			break ;

		case 0b00010000 ... 0b00011111:
			cpu.lcd.column_address = (cpu.lcd.column_address & 0x0F) | (data << 4);

			if (cpu.lcd.column_address > 0x83) cpu.lcd.column_address = 0x83;
			break ;

		case 0b10110000 ... 0b10111111:
			cpu.lcd.page_address = data & 0xF;

			if (cpu.lcd.page_address > 8) cpu.lcd.page_address = 8;
			break ;

		case 0b10100000 ... 0b10100001:
			cpu.lcd.adc_select = data & 1;
			break ;

		case 0b10100110 ... 0b10100111:
			cpu.lcd.reverse_display = data & 1;
			break ;

		case 0b10100100 ... 0b10100101:
			cpu.lcd.all_on = data & 1;
			break ;

		case 0b11100000: // Set RMW mode
			cpu.lcd.rmw_mode = true;
			break ;

		case 0b11101110: // End
			cpu.lcd.rmw_mode = false;
			break ;

		case 0b11000000 ... 0b11001111:
			cpu.lcd.reverse_com_scan = data & 8;
			break ;

		case 0b10000001: // Set electric volume
			cpu.lcd.setting_volume = true;
			break ;
		
		case 0b11100011: // NOP
			break ;

		default:
			dprintf("LCD COMMAND %x", data);
		}
	} else {
		if (cpu.lcd.page_address >= 8) data &= 1;

		cpu.lcd.gddram[cpu.lcd.page_address][cpu.lcd.column_address] = data;

		if (cpu.lcd.column_address < 0x83) {
			cpu.lcd.column_address++;
		}
	}
}
