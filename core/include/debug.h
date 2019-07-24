#pragma once

#include "machine.h"

extern "C" void debug_print(const void* data);

void dprintf(const char* format, ...);
