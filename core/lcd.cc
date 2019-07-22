#include <stdint.h>
#include <string.h>

#include "machine.h"
#include "debug.h"

static const int LCD_WIDTH = 96;
static const int LCD_HEIGHT = 64;

struct ContrastSetting {
	uint8_t on;
	uint8_t off;
};

static const ContrastSetting levels[0x40] = {
	{ 0xFF, 0xFF },	{ 0xF7, 0xFF },	{ 0xEF, 0xFF },	{ 0xE7, 0xFF },
	{ 0xDE, 0xFF },	{ 0xD6, 0xFF },	{ 0xCE, 0xFF },	{ 0xC6, 0xFF },
	{ 0xBD, 0xFF },	{ 0xB5, 0xFF },	{ 0xAD, 0xFF },	{ 0xA5, 0xFF },
	{ 0x9C, 0xFF },	{ 0x94, 0xFF },	{ 0x8C, 0xFF },	{ 0x84, 0xFF },
	{ 0x7B, 0xFF },	{ 0x73, 0xFF },	{ 0x6B, 0xFF },	{ 0x63, 0xFF },
	{ 0x5A, 0xFF },	{ 0x52, 0xFF },	{ 0x4A, 0xFF },	{ 0x42, 0xFF },
	{ 0x39, 0xFF },	{ 0x31, 0xFF },	{ 0x29, 0xFF },	{ 0x21, 0xFF },
	{ 0x18, 0xFF },	{ 0x10, 0xFF },	{ 0x08, 0xFF },	{ 0x00, 0xFF },
	{ 0x00, 0xFF },	{ 0x00, 0xF7 },	{ 0x00, 0xEF },	{ 0x00, 0xE7 },
	{ 0x00, 0xDE },	{ 0x00, 0xD6 },	{ 0x00, 0xCE },	{ 0x00, 0xC6 },
	{ 0x00, 0xBD },	{ 0x00, 0xB5 },	{ 0x00, 0xAD },	{ 0x00, 0xA5 },
	{ 0x00, 0x9C },	{ 0x00, 0x94 },	{ 0x00, 0x8C },	{ 0x00, 0x84 },
	{ 0x00, 0x7B },	{ 0x00, 0x73 },	{ 0x00, 0x6B },	{ 0x00, 0x63 },
	{ 0x00, 0x5A },	{ 0x00, 0x52 },	{ 0x00, 0x4A },	{ 0x00, 0x42 },
	{ 0x00, 0x39 },	{ 0x00, 0x31 },	{ 0x00, 0x29 },	{ 0x00, 0x21 },
	{ 0x00, 0x18 },	{ 0x00, 0x10 },	{ 0x00, 0x08 },	{ 0x00, 0x00 }
};

void lcd_reset(MachineState& cpu) {
	cpu.lcd.rmw_mode = false;	
}

// Generage
EXPORT const void* lcd_render(MachineState& cpu) {
	static uint8_t framebuffer[LCD_HEIGHT][LCD_WIDTH];

	const uint8_t off = levels[cpu.lcd.volume].off;
	const uint8_t on = levels[cpu.lcd.volume].on;

	if (!cpu.lcd.display_enable) {
		return memset(&framebuffer[0][0], 0xFF, sizeof(framebuffer));
	} else if (cpu.lcd.all_on) {
		return memset(&framebuffer[0][0], on, sizeof(framebuffer));
	}

	uint8_t* current_page = cpu.lcd.gddram[0];

	for (int page = 0; page < LCD_HEIGHT; page += 8, current_page += sizeof(cpu.lcd.gddram[0])) {
		for (int x = 0; x < LCD_WIDTH; x++) {
			uint8_t byte =  current_page[cpu.lcd.adc_select ? 131 - x : x];

			if (cpu.lcd.reverse_display) byte = ~byte;

			for (int bit = 0; bit < 8; bit++) {
				int dy = (page + bit);

				if (cpu.lcd.reverse_com_scan) dy = 63 - dy;

				framebuffer[(dy + cpu.lcd.start_address) & 0x3F][x] = ((byte >> bit) & 1) ? on : off;
			}
		}
	}

	return framebuffer;
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
		cpu.lcd.volume = data & 0x3F;
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

	cpu.lcd.updated = true;
}
