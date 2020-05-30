#pragma once

enum TraceType :uint32_t {
    TRACE_INSTRUCTION   = 0b00000001,
    TRACE_IMMEDIATE     = 0b00000010,
    TRACE_BRANCH_TARGET = 0b00000100,
    TRACE_TILE_DATA     = 0b00001000,
    TRACE_SPRITE_DATA   = 0b00010000,
    TRACE_READ          = 0b01000000,
    TRACE_WRITE         = 0b10000000,
};

#ifdef TRACING
extern "C" void trace_access(Machine::State& cpu, uint32_t address, uint32_t kind, uint8_t data = 0);
#else
#define trace_access(...)
#endif
