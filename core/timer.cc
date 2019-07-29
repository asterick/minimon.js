#include <string.h>
#include "machine.h"

void Timer::reset(Machine::State& cpu) {

}

void Timer::clock(Machine::State& cpu, int osc1, int osc3) {

}

uint8_t Timer::read(Machine::State& cpu, uint32_t address) {
	return 0;
}

void Timer::write(Machine::State& cpu, uint8_t data, uint32_t address) {

}
