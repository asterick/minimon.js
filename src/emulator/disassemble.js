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

import * as Table from "../../s1c88.csv";

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

export class Disassembler {
	constructor(system) {
		this._system = system;
	}

	_read8() {
		const data = this._system.read(this._system.translate(this._address++), true);
		this._data.push(data);
		return data;
	}

	_read16() {
		const lo = this._read8();
		return (this._read8() << 8) | lo;
	}

	_sign8() {
		return this._read8() << 24 >> 24;
	}

	_sign16() {
		return this._read16() << 16 >> 16;
	}

	_pcRelative(p) {
		return this._system.translate(p + this._address - 1);
	}

	_processArg(arg) {
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
			return `#${toHex(this._pcRelative(this._sign8()), 6)}h`
		case Table.Argument.REL_16:
			return `#${toHex(this._pcRelative(this._sign16()), 6)}h`
		default:
			throw new Error(arg);
		}
	}

	_processCondition(cond) {
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
			throw new Error(cond);
		}
	}

	next() {
		const address = this._system.translate(this._address);

		this._data = [];

		let inst = Table.InstructionTable;

		do {
			inst = inst[this._read8()];
		} while(Array.isArray(inst));

		if (!inst) return null;

		let { op, condition, args } = inst;

		args = args.map(a => this._processArg(a))

		if (condition !== Table.Condition.NONE) {
			args.unshift(this._processCondition(condition))
		}

		return {
			data: this._data,
			op, args, address
		};
	}

	process(target, rows) {
		const lines = [];
		let next;

		this._address = target;

		while (rows-- > 0 && (next = this.next())) {
			lines.push(next);
		}

		return lines;
	}
}
