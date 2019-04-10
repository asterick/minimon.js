#!/usr/bin/env python3
from json import dumps
import csv

op0s, op1s, op2s = [None] * 0x100, [None] * 0x100, [None] * 0x100
op0s[0xCE] = op1s
op0s[0xCF] = op2s

CONDITIONS = {
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

ARGUMENTS = {
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
    "IP": "REG_EP",
    "XP": "REG_EP",
    "YP": "REG_EP",
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
    "#bb": "IMM_8",
    "#pp": "IMM_8",
    "#hh": "IMM_8",
    "#mmnn": "IMM_16"

}

def format(op, arg1, arg2):
    condition = None
    if arg1 in CONDITIONS:
        condition, arg1, arg2 = CONDITIONS[arg1], arg2, None

    return { 
        "op": op, 
        "condition": condition, 
        "args": [ARGUMENTS[arg] for arg in [arg1, arg2] if arg]
    }

all_ops = []
all_args = []

with open('s1c88.csv', 'r') as csvfile:
    spamreader = csv.reader(csvfile)

    next(spamreader)

    for row in spamreader:
        code, op0, arg0_1, arg0_2, _1, op1, arg1_1, arg1_2, _2, op2, arg2_1, arg2_2 = row
        code = int(code, 16)

        if not op0 in ['[EXPANSION]', 'undefined']:
        	op0s[code] = format(op0, arg0_1, arg0_2)
        if op1 != 'undefined':
        	op1s[code] = format(op1, arg1_1, arg1_2)
        if op2 != 'undefined':
        	op2s[code] = format(op2, arg2_1, arg2_2)

        all_ops += [op0, op1, op2]
        all_args += [arg0_1, arg0_2, arg1_1, arg1_2, arg2_1, arg2_2]

print (dumps(op0s, indent=4))
