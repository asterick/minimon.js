#include <string.h>
#include "machine.h"

uint8_t MASK_STATUS[] = {
	0b11111111,
	0b00110011,
	0b11111111,
	0b00000011,
	0b11111111,
	0b00000011
};

uint8_t MASK_TIMER[] = {
	0b10001111,
	0b00001111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b00000000,
	0b00000000
};

int PRESCALE_OSC1[] = {
	1, 2, 4, 8, 16, 32, 64, 128
};

int PRESCALE_OSC3[] = {
	2, 8, 32, 64, 128, 256, 1024, 4096
};

void Timer::reset(Machine::State& cpu) {

}

void Timer::clock(Machine::State& cpu, int osc1, int osc3) {

}

uint8_t Timer::read(Machine::State& cpu, uint32_t address) {
	switch (address) {
		case 0x2018 ... 0x201D:
			return cpu.timer.status_bytes[address - 0x2018];
	}

	return 0;
}

void Timer::write(Machine::State& cpu, uint8_t data, uint32_t address) {
	switch (address) {
		case 0x2018 ... 0x201D:
			cpu.timer.status_bytes[address - 0x2018] = data & MASK_STATUS[address - 0x2018];
			break ;
	}
}
