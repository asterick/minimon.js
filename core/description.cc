#include <stdint.h>
#include <stddef.h>

#include "machine.h"
#include "debug.h"

enum FieldType : uint32_t {
	TYPE_END,
	TYPE_STRUCT,
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_BOOL
};

struct StructTable {
	FieldType type;
	const char* struct_name;
	const void* field;
	const int* sizes;
};

extern "C" const void* get_description(Machine::State& state) {
	StructTable reg_table[] = {
		{ TYPE_UINT8, "sc", &state.reg.sc },
		{ TYPE_UINT8, "cc", &state.reg.cc },
		{ TYPE_UINT8, "br", &state.reg.br }, 
		{ TYPE_UINT8, "ep", &state.reg.ep }, 
		{ TYPE_UINT8, "xp", &state.reg.xp }, 
		{ TYPE_UINT8, "yp", &state.reg.yp }, 
		{ TYPE_UINT8, "cb", &state.reg.cb }, 
		{ TYPE_UINT8, "nb", &state.reg.nb }, 

		{ TYPE_UINT8, "a", &state.reg.a },
		{ TYPE_UINT8, "b", &state.reg.b },
		{ TYPE_UINT8, "l", &state.reg.l },
		{ TYPE_UINT8, "h", &state.reg.h },

		{ TYPE_UINT16, "ba", &state.reg.ba },
		{ TYPE_UINT16, "hl", &state.reg.hl },
		{ TYPE_UINT16, "pc", &state.reg.pc },
		{ TYPE_UINT16, "sp", &state.reg.sp },
		{ TYPE_UINT16, "ix", &state.reg.ix },
		{ TYPE_UINT16, "iy", &state.reg.iy },

		{ TYPE_END }
	};

	StructTable tim256_table[] = {
		{ TYPE_BOOL, "running", &state.tim256.running },
		{ TYPE_UINT16, "value", &state.tim256.value },

		{ TYPE_END }
	};

	StructTable rtc_table[] = {
		{ TYPE_BOOL, "running", &state.rtc.running },
		{ TYPE_UINT32, "value", &state.rtc.value },

		{ TYPE_END }
	};

	static const int RAM_SIZE[] = { 0x1000, -1 };
	StructTable table[] = {
		{ TYPE_STRUCT, "cpu", reg_table },
		{ TYPE_STRUCT, "rtc", rtc_table },
		{ TYPE_STRUCT, "tim256", tim256_table },

		/*
		IRQ::State irq;
		LCD::State lcd;
		Control::State ctrl;
		Blitter::State blitter;
		Timers::State timers;
		Input::State input;
		GPIO::State gpio;
		*/

		{ TYPE_UINT8, "bus_cap", &state.bus_cap },
		{ TYPE_INT32, "clocks", &state.clocks },
		{ TYPE_INT32, "osc1_overflow", &state.osc1_overflow },
		{ TYPE_BOOL, "sleeping", &state.sleeping },
		{ TYPE_BOOL, "halted", &state.sleeping },
		{ TYPE_UINT8, "ram", &state.ram, RAM_SIZE },
		/*
		union {
		 	Blitter::Overlay overlay;
		};
		*/

		{ TYPE_END }
	};

	dprintf("%x", table);

	return &table[0];
}
