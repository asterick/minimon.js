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

import * as Table from "./table.ts";

// Anything that can serve byte fetches: Minimon satisfies this (reading
// through the live CPU bus), and the debugger document model supplies a
// side-effect-free reader with an explicit bank mapping instead
export interface MemoryAccess {
	read(address: number): number;
	translate(address: number): number;
}

interface Instruction {
	op: string;
	condition: Table.Condition;
	args: Table.Argument[];
}

// The instruction table nests: 0xCE/0xCF prefixes select sub-tables
type TableEntry = Instruction | TableEntry[] | null;

export interface DisassembledLine {
	data: number[];
	op: string;
	args: string[];
	address: number;
}

function toHex(v: number, c: number): string {
	let out = v.toString(16).toUpperCase();
	while (out.length < c) out = "0" + out;
	return out;
}

export class Disassembler {
	private _system: MemoryAccess;
	private _address = 0;
	private _data: number[] = [];

	constructor(system: MemoryAccess) {
		this._system = system;
	}

	private _read8(): number {
		const data = this._system.read(this._system.translate(this._address++));
		this._data.push(data);
		return data;
	}

	private _read16(): number {
		const lo = this._read8();
		return (this._read8() << 8) | lo;
	}

	private _sign8(): number {
		return this._read8() << 24 >> 24;
	}

	private _sign16(): number {
		return this._read16() << 16 >> 16;
	}

	private _pcRelative(p: number): number {
		return this._system.translate(p + this._address - 1);
	}

	private _processArg(arg: Table.Argument): string {
		let val;

		switch (arg) {
		case Table.Argument.REGS_ALL:
			return "ALL";
		case Table.Argument.REGS_ALE:
			return "ALE";
		case Table.Argument.REG_A:
			return "A";
		case Table.Argument.REG_B:
			return "B";
		case Table.Argument.REG_L:
			return "L";
		case Table.Argument.REG_H:
			return "H";
		case Table.Argument.REG_BA:
			return "BA";
		case Table.Argument.REG_HL:
			return "HL";
		case Table.Argument.REG_IX:
			return "IX";
		case Table.Argument.REG_IY:
			return "IY";
		case Table.Argument.REG_NB:
			return "NB";
		case Table.Argument.REG_BR:
			return "BR";
		case Table.Argument.REG_EP:
			return "EP";
		case Table.Argument.REG_IP:
			return "IP";
		case Table.Argument.REG_XP:
			return "XP";
		case Table.Argument.REG_YP:
			return "YP";
		case Table.Argument.REG_SC:
			return "SC";
		case Table.Argument.REG_SP:
			return "SP";
		case Table.Argument.REG_PC:
			return "PC";
		case Table.Argument.MEM_HL:
			return "[HL]";
		case Table.Argument.MEM_IX:
			return "[IX]";
		case Table.Argument.MEM_IY:
			return "[IY]";
		case Table.Argument.MEM_IX_OFF:
			return "[IX+L]";
		case Table.Argument.MEM_IY_OFF:
			return "[IY+L]";
		case Table.Argument.MEM_SP_DISP:
			val = this._sign8();
			return `[SP${val > 0 ? "+" : ""}${val}]`;
		case Table.Argument.MEM_IX_DISP:
			val = this._sign8();
			return `[IX${val > 0 ? "+" : ""}${val}]`;
		case Table.Argument.MEM_IY_DISP:
			val = this._sign8();
			return `[IY${val > 0 ? "+" : ""}${val}]`;
		case Table.Argument.MEM_ABS16:
			return `[${toHex(this._read16(), 4)}h]`;
		case Table.Argument.MEM_BR:
			return `[BR:${toHex(this._read8(), 2)}h]`;
		case Table.Argument.MEM_VECTOR:
			return `[${toHex(this._read8(), 2)}h]`;
		case Table.Argument.IMM_8:
			return `#0${this._read8().toString(16).toUpperCase()}h`;
		case Table.Argument.IMM_16:
			return `#0${this._read16().toString(16).toUpperCase()}h`;
		case Table.Argument.REL_8:
			return `#${toHex(this._pcRelative(this._sign8()), 6)}h`;
		case Table.Argument.REL_16:
			return `#${toHex(this._pcRelative(this._sign16()), 6)}h`;
		default:
			throw new Error(String(arg));
		}
	}

	private _processCondition(cond: Table.Condition): string {
		switch (cond) {
		case Table.Condition.LESS_THAN:
			return "LT";
		case Table.Condition.LESS_EQUAL:
			return "LE";
		case Table.Condition.GREATER_THAN:
			return "GT";
		case Table.Condition.GREATER_EQUAL:
			return "LE";
		case Table.Condition.OVERFLOW:
			return "V";
		case Table.Condition.NOT_OVERFLOW:
			return "NV";
		case Table.Condition.POSITIVE:
			return "P";
		case Table.Condition.MINUS:
			return "M";
		case Table.Condition.CARRY:
			return "C";
		case Table.Condition.NOT_CARRY:
			return "NC";
		case Table.Condition.ZERO:
			return "Z";
		case Table.Condition.NOT_ZERO:
			return "NZ";
		case Table.Condition.SPECIAL_FLAG_0:
			return "F0";
		case Table.Condition.SPECIAL_FLAG_1:
			return "F1";
		case Table.Condition.SPECIAL_FLAG_2:
			return "F2";
		case Table.Condition.SPECIAL_FLAG_3:
			return "F3";
		case Table.Condition.NOT_SPECIAL_FLAG_0:
			return "NF0";
		case Table.Condition.NOT_SPECIAL_FLAG_1:
			return "NF1";
		case Table.Condition.NOT_SPECIAL_FLAG_2:
			return "NF2";
		case Table.Condition.NOT_SPECIAL_FLAG_3:
			return "NF3";
		default:
			throw new Error(String(cond));
		}
	}

	next(): DisassembledLine | null {
		const address = this._system.translate(this._address);

		this._data = [];

		let inst: TableEntry = Table.InstructionTable as TableEntry[];

		do {
			inst = (inst as TableEntry[])[this._read8()];
		} while (Array.isArray(inst));

		if (!inst) return null;

		const { op, condition, args } = inst;

		const formatted = args.map((a) => this._processArg(a));

		if (condition !== Table.Condition.NONE) {
			formatted.unshift(this._processCondition(condition));
		}

		return {
			data: this._data,
			op,
			args: formatted,
			address
		};
	}

	process(target: number, rows: number): DisassembledLine[] {
		const lines: DisassembledLine[] = [];
		let next;

		this._address = target;

		while (rows-- > 0 && (next = this.next())) {
			lines.push(next);
		}

		return lines;
	}
}
