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

static Machine::State machine_state;

extern "C" Machine::State* const get_machine() {
	return &machine_state;
}

static const StructTable reg_table[] = {
	{ TYPE_UINT8, "sc", &machine_state.reg.sc },
	{ TYPE_UINT8, "cc", &machine_state.reg.cc },
	{ TYPE_UINT8, "br", &machine_state.reg.br }, 
	{ TYPE_UINT8, "ep", &machine_state.reg.ep }, 
	{ TYPE_UINT8, "xp", &machine_state.reg.xp }, 
	{ TYPE_UINT8, "yp", &machine_state.reg.yp }, 
	{ TYPE_UINT8, "cb", &machine_state.reg.cb }, 
	{ TYPE_UINT8, "nb", &machine_state.reg.nb }, 

	{ TYPE_UINT8, "a", &machine_state.reg.a },
	{ TYPE_UINT8, "b", &machine_state.reg.b },
	{ TYPE_UINT8, "l", &machine_state.reg.l },
	{ TYPE_UINT8, "h", &machine_state.reg.h },

	{ TYPE_UINT16, "ba", &machine_state.reg.ba },
	{ TYPE_UINT16, "hl", &machine_state.reg.hl },
	{ TYPE_UINT16, "pc", &machine_state.reg.pc },
	{ TYPE_UINT16, "sp", &machine_state.reg.sp },
	{ TYPE_UINT16, "ix", &machine_state.reg.ix },
	{ TYPE_UINT16, "iy", &machine_state.reg.iy },

	{ TYPE_END }
};

static const StructTable tim256_table[] = {
	{ TYPE_BOOL, "running", &machine_state.tim256.running },
	{ TYPE_UINT16, "value", &machine_state.tim256.value },

	{ TYPE_END }
};

static const StructTable rtc_table[] = {
	{ TYPE_BOOL, "running", &machine_state.rtc.running },
	{ TYPE_UINT32, "value", &machine_state.rtc.value },

	{ TYPE_END }
};

static const int RAM_SIZE[] = { 0x1000, -1 };
static const StructTable table[] = {
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

	{ TYPE_UINT8, "bus_cap", &machine_state.bus_cap },
	{ TYPE_INT32, "clocks", &machine_state.clocks },
	{ TYPE_INT32, "osc1_overflow", &machine_state.osc1_overflow },
	{ TYPE_BOOL, "sleeping", &machine_state.sleeping },
	{ TYPE_BOOL, "halted", &machine_state.sleeping },
	{ TYPE_UINT8, "ram", &machine_state.ram, RAM_SIZE },
	/*
	union {
	 	Blitter::Overlay overlay;
	};
	*/

	{ TYPE_END }
};

extern "C" const StructTable* get_description() {
	return table;
}
