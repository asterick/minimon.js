/*
 * MIT License
 *
 * Copyright (c) 2019-2021, Bryon Vandiver
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace EEPROM {
	enum PinState : uint8_t {
		PIN_RESET,
		PIN_SET,
		PIN_FLOAT
	};

	enum I2C_MODE : uint8_t {
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
		int8_t bit;
		uint16_t address;

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
