/*
ISC License

Copyright (c) 2019, Bryon Vandiver

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

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
