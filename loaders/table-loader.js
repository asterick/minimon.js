/**
 * ISC License
 *
 * Copyright (c) 2019, Bryon Vandiver
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

const parse = require('csv-parse/lib/sync');

const CONDITIONS = {
    "\xFF": "NONE",
    "LT": "LESS_THAN",
    "LE": "LESS_EQUAL",
    "GT": "GREATER_THAN",
    "GE": "GREATER_EQUAL",
    "V": "OVERFLOW",
    "NV": "NOT_OVERFLOW",
    "P": "POSITIVE",
    "M": "MINUS",
    "C": "CARRY",
    "NC": "NOT_CARRY",
    "Z": "ZERO",
    "NZ": "NOT_ZERO",

    "F0": "SPECIAL_FLAG_0",
    "F1": "SPECIAL_FLAG_1",
    "F2": "SPECIAL_FLAG_2",
    "F3": "SPECIAL_FLAG_3",
    "NF0": "NOT_SPECIAL_FLAG_0",
    "NF1": "NOT_SPECIAL_FLAG_1",
    "NF2": "NOT_SPECIAL_FLAG_2",
    "NF3": "NOT_SPECIAL_FLAG_3"
}

const ARGUMENTS = {
    "ALL": "REGS_ALL",
    "ALE": "REGS_ALE",

    "A": "REG_A",
    "B": "REG_B",
    "L": "REG_L",
    "H": "REG_H",

    "BA": "REG_BA",
    "HL": "REG_HL",
    "IX": "REG_IX",
    "IY": "REG_IY",

    "NB": "REG_NB",
    "BR": "REG_BR",
    "EP": "REG_EP",
    "IP": "REG_IP",
    "XP": "REG_XP",
    "YP": "REG_YP",
    "SC": "REG_SC",

    "SP": "REG_SP",
    "PC": "REG_PC",

    "[hhll]": "MEM_ABS16",
    "[HL]": "MEM_HL",
    "[IX]": "MEM_IX",
    "[IY]": "MEM_IY",
    "[SP+dd]": "MEM_SP_DISP",
    "[IX+dd]": "MEM_IX_DISP",
    "[IY+dd]": "MEM_IY_DISP",
    "[IX+L]": "MEM_IX_OFF",
    "[IY+L]": "MEM_IY_OFF",
    "[BR:ll]": "MEM_BR",
    "[kk]": "MEM_VECTOR",

    "rr": "REL_8",
    "qqrr": "REL_16",
    "#nn": "IMM_8",
    "#mmnn": "IMM_16"
}

function format(op, arg1, arg2) {
    if (op == 'undefined')
        return null;

    let condition = "NONE";
    let args = [];

    if (CONDITIONS[arg1]) {
        condition = CONDITIONS[arg1]
        arg1 = arg2;
        arg2 = null;
    }

    if (ARGUMENTS[arg1]) args.push(ARGUMENTS[arg1]);
    if (ARGUMENTS[arg2]) args.push(ARGUMENTS[arg2]);

    return { "op": op, "condition": condition, "args": args }
}

function display(table) {
    if (Array.isArray(table)) {
        return `[${table.map(display).join(",")}]`;
    } else if (table) {
        const { op, args, condition } = table;
        return `{
            op: "${op}",
            condition: Condition.${condition},
            args: [${args.map((v) => `Argument.${v}`).join(",")}]
        }`;
    } else {
        return "null";
    }
}

module.exports = function(source) {
    let op0s = Array(0x100);
    let op1s = Array(0x100);
    let op2s = Array(0x100);

    op0s[0xCE] = op1s;
    op0s[0xCF] = op2s;

    const records = parse(source, {
        columns: true,
        skip_empty_lines: true
    });

    for (record of records) {
        let { code, cycles0, op0, arg0_1, arg0_2, cycles1, op1, arg1_1, arg1_2, cycles2, op2, arg2_1, arg2_2 } = record;
        code = parseInt(code, 16);

        op0s[code] = format(op0, arg0_1, arg0_2)
        op1s[code] = format(op1, arg1_1, arg1_2)
        op2s[code] = format(op2, arg2_1, arg2_2)
    }

    return `export const Argument = {
    ${Object.values(ARGUMENTS).map((v, i) => `${v}: ${i}`).join(",\t\n\r")}
    };

    export const Condition = {
    ${Object.values(CONDITIONS).map((v, i) => `${v}: ${i}`).join(",\t\n\r")}
    };

    export const InstructionTable = ${display(op0s)};
    `
}
