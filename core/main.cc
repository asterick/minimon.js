#include "system.h"

static ProcessorState minimon;

__attribute__ ((visibility ("default"))) extern "C"
ProcessorState* const get_machine() {
	return &minimon;
}