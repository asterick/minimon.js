#include <string.h>
#include "machine.h"

static const uint8_t EEPROM_CLOCK = 0b1000;
static const uint8_t EEPROM_DATA = 0b0100;

void GPIO::reset(GPIO::State& gpio) {
	memset(&gpio, 0, sizeof(gpio));

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
