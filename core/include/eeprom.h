#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace EEPROM {
	enum PinState {
		PIN_FLOAT,
		PIN_RESET,
		PIN_SET
	};

	enum I2C_MODE {
		SYSTEM_STOP,
		
		SYSTEM_SELECT,
		SYSTEM_ADDRESS_H,
		SYSTEM_ADDRESS_L,
		SYSTEM_READ,
		SYSTEM_WRITE
	};

	struct State {
		uint8_t data[0x2000];

		I2C_MODE mode;
		uint8_t shift;
		int bit;
		unsigned address:13;

		PinState data_in;
		PinState data_out;
		PinState clock_in;
	};

	void reset(EEPROM::State&);
	void setDataPin(EEPROM::State&, PinState data);
	void setClockPin(EEPROM::State&, PinState clock);
	bool getDataPin(EEPROM::State&);
	bool getClockPin(EEPROM::State&);
}
