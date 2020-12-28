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
