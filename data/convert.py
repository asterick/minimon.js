#!/usr/bin/env python2
from json import dumps
import csv

op0s, op1s, op2s = [None] * 0x100, [None] * 0x100, [None] * 0x100
op0s[0xCE] = op1s
op0s[0xCF] = op2s

def format(op, arg1, arg2):
	return { "op": op, "args": [arg for arg in arg1, arg2 if arg]}

with file('s1c88.csv', 'rb') as csvfile:
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

print dumps(op0s, indent=4)