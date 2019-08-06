#include <stdint.h>
#include <stddef.h>

#include "machine.h"
#include "debug.h"

enum FieldType : uint32_t {
	TYPE_END,
	TYPE_STRUCT,
	TYPE_UINT8,
	TYPE_UINT16,
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

	StructTable table[] = {
		{ TYPE_STRUCT, "cpu", reg_table },

		{ TYPE_END }
	};

	dprintf("%x", table);

	return &table[0];
}
