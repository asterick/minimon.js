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

#include <stdint.h>

namespace Machine { struct State; };

namespace Timers {
	// lo_flags / hi_flags bits
	const uint8_t TIMER_INPUT   = 0x01;
	const uint8_t TIMER_PRESET  = 0x02;
	const uint8_t TIMER_RUNNING = 0x04;
	const uint8_t TIMER_OUTPUT  = 0x08;
	const uint8_t TIMER_MODE16  = 0x80;	// lo_flags only

	struct Timer {
		uint8_t lo_flags;
		uint8_t hi_flags;

		uint16_t preset;
		uint16_t compare;
		uint16_t count;

		int  lo_clock_ratio;
		bool lo_clock_ctrl;
		bool lo_clock_source;

		int  hi_clock_ratio;
		bool hi_clock_ctrl;
		bool hi_clock_source;

		bool mode16() const {
			return (lo_flags & TIMER_MODE16) != 0;
		}

		bool lo_running() const {
			return (lo_flags & TIMER_RUNNING) != 0;
		}

		bool hi_running() const {
			return (hi_flags & TIMER_RUNNING) != 0;
		}

		// The guest's 8-byte register window (0x2030/0x2038/0x2048):
		// flag bytes, then preset/compare/count as little-endian pairs
		uint8_t reg_read(int index) const {
			switch (index & 7) {
			case 0:  return lo_flags;
			case 1:  return hi_flags;
			case 2:  return (uint8_t)preset;
			case 3:  return preset >> 8;
			case 4:  return (uint8_t)compare;
			case 5:  return compare >> 8;
			case 6:  return (uint8_t)count;
			default: return count >> 8;
			}
		}

		void reg_write(int index, uint8_t value) {
			switch (index & 7) {
			case 0:  lo_flags = value; break;
			case 1:  hi_flags = value; break;
			case 2:  preset  = (preset  & 0xFF00) | value; break;
			case 3:  preset  = (preset  & 0x00FF) | (value << 8); break;
			case 4:  compare = (compare & 0xFF00) | value; break;
			case 5:  compare = (compare & 0x00FF) | (value << 8); break;
			case 6:  count   = (count   & 0xFF00) | value; break;
			default: count   = (count   & 0x00FF) | (value << 8); break;
			}
		}
	};

	struct State {
		Timer timer[3];

		bool osc1_enable;
		bool osc3_enable;

		uint32_t osc1_prescale;
		uint32_t osc3_prescale;
	};

	void reset(Machine::State& cpu);
	void clock(Machine::State& cpu, int osc1, int osc3);
	uint8_t read(Machine::State& cpu, uint32_t address);
	void write(Machine::State& cpu, uint8_t data, uint32_t address);
};
