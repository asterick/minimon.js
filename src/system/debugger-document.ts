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

/*
 * The debugger's document model: turns the CPU's 64K view — BIOS, RAM,
 * hardware registers, bank 0 of the cartridge in the lower half, a
 * selected bank in the upper half — into a list of renderable lines
 * using the core's classification maps.
 *
 * A byte the CPU has executed is code (one line per instruction,
 * disassembled); a byte it has only read/written is data; a byte it has
 * never touched is unknown. Data and unknown render as 16-byte hex rows
 * aligned to 16-byte boundaries (rows break early where classification
 * changes). RAM and hardware registers are always data.
 *
 * All reads here are side-effect-free: rendering must never go through
 * cpu_read, which captures the bus.
 */

import { Disassembler } from "./disassemble";
import { TRACING_FLAGS, type Minimon } from "./index";

export type LineKind = 'code' | 'data' | 'unknown';

export interface DocumentLine {
	kind: LineKind;
	/** CPU-visible address (0x0000-0xFFFF) */
	address: number;
	/** physical address after bank mapping */
	physical: number;
	length: number;

	/** disassembly, code lines only */
	op?: string;
	args?: string[];
}

export interface DebuggerDocument {
	bank: number;
	lines: DocumentLine[];
	/** index of the line containing a CPU address */
	lineAt(address: number): number;
	/** side-effect-free byte read for rendering hex rows */
	byteAt(address: number): number | null;
}

const BANK_WINDOW = 0x8000;
const HEX_ROW = 16;

// Classification flags that mean "the CPU executed this byte"
const CODE_FLAGS = TRACING_FLAGS.INSTRUCTION;

function physical(address: number, bank: number): number {
	return (address & 0x8000) ? ((address & 0x7FFF) | (bank * BANK_WINDOW)) : address;
}

export function buildDocument(system: Minimon, bank: number): DebuggerDocument {
	const { biosTrace, cartridgeTrace, bios, state } = system;

	// null = unreadable for rendering (hardware registers are computed
	// on read; reading them perturbs the machine)
	function byteAt(address: number): number | null {
		const p = physical(address, bank);

		if (p < 0x1000) return bios[p];
		if (p < 0x2000) return state.ram[p - 0x1000];
		if (p < 0x2100) return null;
		return state.cartridge[p & 0x1FFFFF];
	}

	function classify(address: number): LineKind {
		const p = physical(address, bank);

		if (p < 0x1000) {
			const flags = biosTrace[p];
			if (flags & CODE_FLAGS) return 'code';
			return flags ? 'data' : 'unknown';
		}

		// RAM and hardware registers are always data
		if (p < 0x2100) return 'data';

		const flags = cartridgeTrace[p & 0x1FFFFF];
		if (flags & CODE_FLAGS) return 'code';
		return flags ? 'data' : 'unknown';
	}

	// Code lines decode through the shared disassembler with an
	// explicit bank mapping and side-effect-free reads
	const disasm = new Disassembler({
		translate: (address) => physical(address, bank),
		read: (p) => {
			if (p < 0x1000) return bios[p];
			if (p < 0x2000) return state.ram[p - 0x1000];
			if (p < 0x2100) return 0;
			return state.cartridge[p & 0x1FFFFF];
		}
	});

	const lines: DocumentLine[] = [];
	// Line index of each 256-byte page start, for O(1)-ish address lookup
	const pageIndex = new Int32Array(0x100).fill(-1);

	let address = 0;

	while (address <= 0xFFFF) {
		const page = address >> 8;
		if (pageIndex[page] < 0) pageIndex[page] = lines.length;

		const kind = classify(address);

		if (kind === 'code') {
			const line = disasm.process(address, 1)[0];

			if (line) {
				lines.push({
					kind, address,
					physical: physical(address, bank),
					length: line.data.length,
					op: line.op,
					args: line.args,
				});
				address += line.data.length;
				continue;
			}
			// Undecodable despite the flag (e.g. table gap): fall
			// through and emit it as a data row instead
		}

		// Hex row: up to the next 16-byte boundary, breaking early if
		// the classification changes
		const rowEnd = Math.min((address & ~(HEX_ROW - 1)) + HEX_ROW, 0x10000);
		let end = address + 1;
		while (end < rowEnd && classify(end) === kind) end++;

		lines.push({
			kind: kind === 'code' ? 'data' : kind,
			address,
			physical: physical(address, bank),
			length: end - address,
		});
		address = end;
	}

	function lineAt(target: number): number {
		let index = pageIndex[(target >> 8) & 0xFF];
		if (index < 0) index = 0;

		while (index < lines.length - 1 && lines[index].address + lines[index].length <= target) index++;
		return index;
	}

	return { bank, lines, lineAt, byteAt };
}

/*
 * Memoizes a document per (bank, state version): the trace maps mutate
 * continuously while the machine runs, so consumers pass the system's
 * version counter as the invalidation key (one rebuild per UI frame at
 * worst).
 */
export class DocumentCache {
	private _document: DebuggerDocument | null = null;
	private _bank = -1;
	private _version = -1;

	get(system: Minimon, bank: number, version: number): DebuggerDocument {
		if (!this._document || bank !== this._bank || version !== this._version) {
			this._document = buildDocument(system, bank);
			this._bank = bank;
			this._version = version;
		}

		return this._document;
	}
}
