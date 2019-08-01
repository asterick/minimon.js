#include <string.h>
#include "machine.h"

void GPIO::reset(GPIO::State& gpio) {
	memset(&gpio, 0, sizeof(gpio));

	EEPROM::reset(gpio.eeprom);
}

static uint8_t getBusState(GPIO::State& gpio) {
	uint8_t inputs = 0b01000000 & ~gpio.direction;
	uint8_t outputs = gpio.output & gpio.direction;

	return inputs | outputs
		| (EEPROM::getDataPin(gpio.eeprom) ? 0b0100 : 0)
		| (EEPROM::getClockPin(gpio.eeprom) ? 0b1000 : 0);
}

uint8_t GPIO::read(GPIO::State& gpio, uint32_t address) {
	switch (address) {
	case 0x2070:
		return gpio.direction;
	case 0x2071:
		return getBusState(gpio);
	case 0x2072:
		return gpio.unknown;
	default:
		return 0;
	}
}

void GPIO::write(GPIO::State& gpio, uint8_t data, uint32_t address) {
	switch (address) {
	case 0x2070:
		gpio.direction = data;
		break ;
	case 0x2071:
		gpio.output = data;
		break ;
	case 0x2072:
		gpio.unknown = data & 0xF0;
		break ;
	default:
		break ;
	}

	if (gpio.direction & 0b0100) {
		EEPROM::setDataPin(gpio.eeprom, EEPROM::PIN_FLOAT);
	} else if (gpio.output & 0b0100) {
		EEPROM::setDataPin(gpio.eeprom, EEPROM::PIN_SET);
	} else {
		EEPROM::setDataPin(gpio.eeprom, EEPROM::PIN_RESET);
	}

	if (gpio.direction & 0b1000) {
		EEPROM::setClockPin(gpio.eeprom, EEPROM::PIN_FLOAT);
	} else if (gpio.output & 0b1000) {
		EEPROM::setClockPin(gpio.eeprom, EEPROM::PIN_SET);
	} else {
		EEPROM::setClockPin(gpio.eeprom, EEPROM::PIN_RESET);
	}
}
