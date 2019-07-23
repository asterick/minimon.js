#pragma once

#include <stdint.h>

namespace Machine { struct State; };

namespace LCD {
	struct State {
		uint8_t gddram[9][132];
		uint8_t read_buffer;
		uint8_t volume;
		uint8_t column_address;
		uint8_t page_address;
		uint8_t start_address;

		bool	rmw_mode;
		bool 	adc_select;
		bool	setting_volume;
		bool	display_enable;
		bool	reverse_display;
		bool	all_on;
		bool	reverse_com_scan;

		bool	updated;
	};

	void reset(Machine::State& cpu);

	const void* render(Machine::State& cpu);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
}
