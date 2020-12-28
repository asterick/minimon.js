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

#include <string.h>
#include "machine.h"

static const uint8_t EEPROM_CLOCK = 0b1000;
static const uint8_t EEPROM_DATA = 0b0100;

void GPIO::reset(GPIO::State& gpio) {
	gpio.output = 0;
	gpio.direction = 0;
	gpio.unknown = 0;

	EEPROM::reset(gpio.eeprom);
}

static uint8_t getBusState(GPIO::State& gpio) {
	uint8_t inputs = 0
		| (EEPROM::getClockPin(gpio.eeprom) ? EEPROM_CLOCK : 0)
		| (EEPROM::getDataPin(gpio.eeprom)  ? EEPROM_DATA : 0);

	return (inputs & ~gpio.direction) | (gpio.output & gpio.direction);
}

uint8_t GPIO::read(GPIO::State& gpio, uint32_t address) {
	switch (address) {
	case 0x2060:
		return gpio.direction;
	case 0x2061:
		return getBusState(gpio);
	case 0x2062:
		return gpio.unknown;
	default:
		return 0;
	}
}

void GPIO::write(GPIO::State& gpio, uint8_t data, uint32_t address) {
	switch (address) {
	case 0x2060:
		gpio.direction = data;
		break ;
	case 0x2061:
		gpio.output = data;
		break ;
	case 0x2062:
		gpio.unknown = data & 0xF0;
		break ;
	default:
		break ;
	}

	if (gpio.direction & EEPROM_DATA) {
		EEPROM::setDataPin(gpio.eeprom, (gpio.output & EEPROM_DATA) ? EEPROM::PIN_SET :  EEPROM::PIN_RESET);
	} else {
		EEPROM::setDataPin(gpio.eeprom, EEPROM::PIN_FLOAT);
	}

	if (gpio.direction & EEPROM_CLOCK) {
		EEPROM::setClockPin(gpio.eeprom, (gpio.output & EEPROM_CLOCK) ? EEPROM::PIN_SET :  EEPROM::PIN_RESET);
	} else {
		EEPROM::setClockPin(gpio.eeprom, EEPROM::PIN_FLOAT);
	}
}
