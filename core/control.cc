#include <stdint.h>

#include "control.h"

void Control::reset(Control::State& ctrl) {
	ctrl.data[0] = 0;
	ctrl.data[1] = 0;
	ctrl.data[2] = 0;
}

uint8_t Control::read(Control::State& ctrl, uint32_t address) {
	return ctrl.data[address & 3];
}

void Control::write(Control::State& ctrl, uint8_t data, uint32_t address) {
	ctrl.data[address & 3] = data;
}
