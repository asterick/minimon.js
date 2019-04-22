import * as Table from "./table.js";

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class Disassembler {
	constructor(system) {
		this._system = system;
	}

	_read8() {
		const data = this._system.read(this._address++, true);
		this._data.push(data);
		return data;
	}

	_read16() {
		const lo = this._read8();
		return (this._read8() << 8) | lo;
	}

	_sign8() {
		return _read8() << 24 >> 24;
	}

	_sign16() {
		return _read16() << 16 >> 16;
	}

	_pcRelative(p) {
		return this._system.translate(p + this._address - 1);
	}	

	_processArg(arg) {
		let val;

		switch (arg) {
		case Table.REGS_ALL:
			return "ALL";
		case Table.REGS_ALE:
			return "ALE";
		case Table.REG_A:
			return "A";
		case Table.REG_B:
			return "B";
		case Table.REG_L:
			return "L";
		case Table.REG_H:
			return "H";
		case Table.REG_BA:
			return "BA";
		case Table.REG_HL:
			return "HL";
		case Table.REG_IX:
			return "IX";
		case Table.REG_IY:
			return "IY";
		case Table.REG_NB:
			return "NB";
		case Table.REG_BR:
			return "BR";
		case Table.REG_EP:
			return "EP";
		case Table.REG_IP:
			return "IP";
		case Table.REG_XP:
			return "XP";
		case Table.REG_YP:
			return "YP";
		case Table.REG_SC:
			return "SC";
		case Table.REG_SP:
			return "SP";
		case Table.REG_PC:
			return "PC";
		case Table.MEM_HL:
			return "[HL]";
		case Table.MEM_IX:
			return "[IX]";
		case Table.MEM_IY:
			return "[IY]";
		case Table.MEM_IX_OFF:
			return "[IY+L]";
		case Table.MEM_IY_OFF:
			return "[IY+L]";
		case Table.MEM_SP_DISP:
			val = this._sign8();
			return `[SP${val > 0 ? "+" : ""}${val}]`;
		case Table.MEM_IX_DISP:
			val = this._sign8();
			return `[IX${val > 0 ? "+" : ""}${val}]`;
		case Table.MEM_IY_DISP:
			val = this._sign8();
			return `[IY${val > 0 ? "+" : ""}${val}]`;
		case Table.MEM_ABS16:
			return `[${toHex(this._read16(), 4)}]`;
		case Table.MEM_BR:
			return `[BR:${toHex(this._read8(), 2)}]`;
		case Table.MEM_VECTOR:
			return `[${toHex(this._read8(), 2)}]`;
		case Table.IMM_8:
			return `#${this._read8().toString(16)}`;
		case Table.IMM_16:
			return `#${this._read16().toString(16)}`;
		case Table.REL_8:
			return `#${toHex(this._pcRelative(this._read8()), 6)}`
		case Table.REL_16:
			return `#${toHex(this._pcRelative(this._read16()), 6)}`
		default:
			throw new Error(arg);
		}
	}

	_processCondition(cond) {
		switch (cond) {
		case CONDITION_LESS_THAN:
			return "LT";
		case CONDITION_LESS_EQUAL:
			return "LE";
		case CONDITION_GREATER_THAN:
			return "GT";
		case CONDITION_GREATER_EQUAL:
			return "LE";
		case CONDITION_OVERFLOW:
			return "V";
		case CONDITION_NOT_OVERFLOW:
			return "NV";
		case CONDITION_POSITIVE:
			return "P";
		case CONDITION_MINUS:
			return "M";
		case CONDITION_CARRY:
			return "C";
		case CONDITION_NOT_CARRY:
			return "NC";
		case CONDITION_ZERO:
			return "Z";
		case CONDITION_NOT_ZERO:
			return "NZ";
		case CONDITION_SPECIAL_FLAG_0:
			return "F0";
		case CONDITION_SPECIAL_FLAG_1:
			return "F1";
		case CONDITION_SPECIAL_FLAG_2:
			return "F2";
		case CONDITION_SPECIAL_FLAG_3:
			return "F3";
		case CONDITION_NOT_SPECIAL_FLAG_0:
			return "NF0";
		case CONDITION_NOT_SPECIAL_FLAG_1:
			return "NF1";
		case CONDITION_NOT_SPECIAL_FLAG_2:
			return "NF2";
		case CONDITION_NOT_SPECIAL_FLAG_3:
			return "NF3";
		default:
			throw new Error(cond);
		}
	}

	process(target, rows) {
		const lines = [];

		this._address = target;

		while (lines.length < rows) {
			const address = this._address;

			this._data = [];

			let inst = Table.INSTRUCTION_TABLE;

			do {
				inst = inst[this._read8()];
			} while(Array.isArray(inst));

			if (!inst) break ;

			let { op, condition, args } = inst;

			args = args.map(a => this._processArg(a))

			if (condition !== Table.CONDITION_NONE) {
				args.shift(this._processArg(condition))
			}

			lines.push({
				data: this._data,
				op, args, address
			});
		}

		return lines;
	}
}

export default Disassembler;
