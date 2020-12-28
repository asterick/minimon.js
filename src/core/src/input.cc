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

// TODO: HANDLE DEBOUNCE REGISTERS

static const IRQ::Vector vectors[] = {
	IRQ::IRQ_K00,
	IRQ::IRQ_K01,
	IRQ::IRQ_K02,
	IRQ::IRQ_K03,
	IRQ::IRQ_K04,
	IRQ::IRQ_K05,
	IRQ::IRQ_K06,
	IRQ::IRQ_K07,
	IRQ::IRQ_K10,
	IRQ::IRQ_K11
};

void Input::reset(Input::State& inputs) {
	memset(&inputs, 0, sizeof(inputs));
	inputs.input_state = 0b1111111111;
}

void Input::update(Machine::State& cpu, uint16_t value) {
	uint16_t trigger = (value ^ cpu.input.input_state) & (value ^ cpu.input.interrupt_direction);

	cpu.input.input_state = value;

	for (int i = 0; i < 10; i++) {
		if (trigger & (1 << i)) IRQ::trigger(cpu, vectors[i]);
	}
}

uint8_t Input::read(Input::State& inputs, uint32_t address) {
	return inputs.bytes[address - 0x2050];
}

void Input::write(Input::State& inputs, uint8_t data, uint32_t address) {
	switch (address) {
	case 0x2050:
		inputs.bytes[0] = data;
		break ;
	case 0x2051:
		inputs.bytes[1] = data & 0b00000011;
		break ;
	case 0x2054:
		inputs.bytes[4] = data & 0b01110111;
		break ;
	case 0x2055:
		inputs.bytes[5] = data & 0b00000111;
		break ;
	}
}
