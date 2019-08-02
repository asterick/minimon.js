#!/usr/bin/env python3

# ISC License
# 
# Copyright (c) 2019, Bryon Vandiver
# 
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
# 
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

from json import dumps
import csv

op0s, op1s, op2s = [None] * 0x100, [None] * 0x100, [None] * 0x100
op0s[0xCE] = op1s
op0s[0xCF] = op2s

CONDITIONS = {
    "\xFF": "CONDITION_NONE",
    "LT": "CONDITION_LESS_THAN",
    "LE": "CONDITION_LESS_EQUAL",
    "GT": "CONDITION_GREATER_THAN",
    "GE": "CONDITION_GREATER_EQUAL",
    "V": "CONDITION_OVERFLOW",
    "NV": "CONDITION_NOT_OVERFLOW",
    "P": "CONDITION_POSITIVE",
    "M": "CONDITION_MINUS",
    "C": "CONDITION_CARRY",
    "NC": "CONDITION_NOT_CARRY",
    "Z": "CONDITION_ZERO",
    "NZ": "CONDITION_NOT_ZERO",

    "F0": "CONDITION_SPECIAL_FLAG_0",
    "F1": "CONDITION_SPECIAL_FLAG_1",
    "F2": "CONDITION_SPECIAL_FLAG_2",
    "F3": "CONDITION_SPECIAL_FLAG_3",
    "NF0": "CONDITION_NOT_SPECIAL_FLAG_0",
    "NF1": "CONDITION_NOT_SPECIAL_FLAG_1",
    "NF2": "CONDITION_NOT_SPECIAL_FLAG_2",
    "NF3": "CONDITION_NOT_SPECIAL_FLAG_3"
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

def format(op, arg1, arg2):
    condition = None
    
    if arg1 in CONDITIONS:
        condition, arg1, arg2 = CONDITIONS[arg1], arg2, None

    args = [ARGUMENTS[arg] for arg in [arg1, arg2] if arg]

    # add conditions
    return { "op": op, "condition": condition or "CONDITION_NONE", "args": args }

def display(table, tabs = ""):
    if type(table) == dict:
        return '%s{ "op": "%s", "condition": %s, "args": [ %s ] }' % ( tabs, table['op'], table['condition'], ", ".join(table["args"]))
        return "%sggg" % tabs
    elif type(table) == list:
        return "%s[\n%s\n%s]" % (tabs, (",\n").join([ display(t, tabs + "\t") for t in table ]), tabs)
    else:
        return "%snull" % tabs

all_ops = []
all_args = []

with open('s1c88.csv', 'r') as csvfile:
    spamreader = csv.reader(csvfile)

    next(spamreader)

    for row in spamreader:
        code, cycles0, op0, arg0_1, arg0_2, cycles1, op1, arg1_1, arg1_2, cycles2, op2, arg2_1, arg2_2 = row
        code = int(code, 16)

        if not op0 in ['[EXPANSION]', 'undefined']:
        	op0s[code] = format(op0, arg0_1, arg0_2)
        if op1 != 'undefined':
        	op1s[code] = format(op1, arg1_1, arg1_2)
        if op2 != 'undefined':
        	op2s[code] = format(op2, arg2_1, arg2_2)

        all_ops += [op0, op1, op2]
        all_args += [arg0_1, arg0_2, arg1_1, arg1_2, arg2_1, arg2_2]

for i, (k, v) in enumerate(ARGUMENTS.items()):
    print ("export const %s = %i;" % (v, i + 1))
print ()

for i, (k, v) in enumerate(CONDITIONS.items()):
    print ("export const %s = %i;" % (v, i + 1))
print ()

print ("export const INSTRUCTION_TABLE =\n%s;" % display(op0s))
