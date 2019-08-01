#include <string.h>

#include "eeprom.h"

void EEPROM::reset(EEPROM::State& state) {
	memset(&state.data, 0, sizeof(state.data));

	state.data_in = PIN_FLOAT;
	state.data_out = PIN_FLOAT;
	state.clock_in = PIN_FLOAT;
}

void EEPROM::setClockPin(EEPROM::State& state, PinState clock) {
	bool data = getDataPin(state);
	bool clock_then = getClockPin(state);

	state.clock_in = clock;

	bool clock_now = getClockPin(state);


	if (clock_then == clock_now) {
		return ;
	}

	if (clock_now) {
		state.shift = (state.shift << 1) | data;
	} else {
		switch (++state.bit) {
		case 0 ... 7:
			if (state.mode == SYSTEM_READ) {
				state.data_out = (state.data[state.address] & (0x80 >> state.bit)) ? PIN_FLOAT : PIN_RESET;
			} else {
				state.data_out = PIN_FLOAT;
			}
			break ;
		case 8:	// Data inbound
			switch (state.mode) {
			case SYSTEM_STOP:
				state.data_out = PIN_FLOAT;
				break ;
			case SYSTEM_SELECT:
				if (state.shift == 0xA0) {
					state.mode = SYSTEM_ADDRESS_H;
					state.data_out = PIN_RESET;
				} else if (state.shift == 0xA1) {
					state.mode = SYSTEM_READ;
					state.data_out = PIN_RESET;
				} else {
					state.mode = SYSTEM_STOP;
					state.data_out = PIN_FLOAT;
				}
				break ;
			case SYSTEM_ADDRESS_H:
				state.address = state.shift << 8;
				state.data_out = PIN_RESET;
				state.mode = SYSTEM_ADDRESS_L;
				break ;
			case SYSTEM_ADDRESS_L:
				state.address |= state.shift;
				state.data_out = PIN_RESET;
				state.mode = SYSTEM_WRITE;
				break ;
			case SYSTEM_WRITE:
				state.data[state.address++] = state.shift;
				state.data_out = PIN_RESET;
				break ;
			case SYSTEM_READ:
				state.mode = SYSTEM_STOP;
				state.data_out = PIN_FLOAT;
				break ;
			}

			state.bit = -1;
			break ;
		}
	}
}

void EEPROM::setDataPin(EEPROM::State& state, PinState data) {
	bool clock = getClockPin(state);
	bool data_then = getDataPin(state);

	state.data_in = data;

	bool data_now = getDataPin(state);

	if (!clock || data_then == data_now) {
		return ;
	}

	if (data_now) {
		state.mode = SYSTEM_STOP;
		state.data_out = PIN_FLOAT;
	} else {
		state.mode = SYSTEM_SELECT;
		state.bit = -1;
	}
}

bool EEPROM::getClockPin(EEPROM::State& state) {
	return (state.clock_in != PIN_RESET);
}

bool EEPROM::getDataPin(EEPROM::State& state) {
	return (state.data_in != PIN_RESET) && (state.data_out != PIN_RESET);
}
