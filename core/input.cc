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
}

void Input::update(Machine::State& cpu, uint16_t value) {
	uint16_t change = value ^ cpu.input.input_state;
	cpu.input.input_state = change;

	uint16_t trigger = change & (value ^ cpu.input.interrupt_direction);

	for (int i = 0; i < 10; i++) {
		if (trigger & (1 << i)) IRQ::trigger(cpu, vectors[i]);
	}
}

uint8_t Input::read(Input::State& inputs, uint32_t address) {
	return inputs.input_bytes[address - 0x2050];
}

void Input::write(Input::State& inputs, uint8_t data, uint32_t address) {
	switch (address) {
	case 0x2050:
		inputs.input_bytes[0] = data;
		break ;	
	case 0x2051:
		inputs.input_bytes[1] = data & 0b00000011;
		break ;	
	case 0x2054:
		inputs.input_bytes[4] = data & 0b01110111;
		break ;	
	case 0x2055:
		inputs.input_bytes[5] = data & 0b00000111;
		break ;	
	}
}
