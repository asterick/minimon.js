#include "machine.h"

static const uint8_t BIT_MASK[] = {
	0b00111111,
	0b00001111,
	0b11111000,
	0b11111111,
	0b00011111,
	0b01111111,
	0b01111111,
	0b11000000,
	0b11111111,
	0b00011111,
};

void Blitter::reset(Machine::State& cpu) {

}

void Blitter::clock(Machine::State& cpu, int osc3) {

}

uint8_t Blitter::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2080:
			return 0 ;
		case 0x2081:
			return 0 ;
		case 0x2082:
			return cpu.blitter.map_bytes[0];
		case 0x2083:
			return cpu.blitter.map_bytes[1];
		case 0x2084:
			return cpu.blitter.map_bytes[2];
		case 0x2085:
			return cpu.blitter.scroll_y;
		case 0x2086:
			return cpu.blitter.scroll_x;
		case 0x2087:
			return cpu.blitter.sprite_bytes[0];
		case 0x2088:
			return cpu.blitter.sprite_bytes[1];
		case 0x2089:
			return cpu.blitter.sprite_bytes[2];
		case 0x208A:
			return 0 ;
	}

	return 0;
}

void Blitter::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	data &= BIT_MASK[address & 0xF];

	switch (address) {
		case 0x2080:
			break ;
		case 0x2081:
			break ;
		case 0x2082:
			cpu.blitter.map_bytes[0] = data;
			break ;
		case 0x2083:
			cpu.blitter.map_bytes[1] = data;
			break ;
		case 0x2084:
			cpu.blitter.map_bytes[2] = data;
			break ;
		case 0x2085:
			cpu.blitter.scroll_y = data;
			break ;
		case 0x2086:
			cpu.blitter.scroll_x = data;
			break ;
		case 0x2087:
			cpu.blitter.sprite_bytes[0] = data;
			break ;
		case 0x2088:
			cpu.blitter.sprite_bytes[1] = data;
			break ;
		case 0x2089:
			cpu.blitter.sprite_bytes[2] = data;
			break ;
	}
}
