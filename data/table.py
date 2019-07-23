#!/usr/bin/env python3
from json import dumps
import os
import csv

CSV_LOCATION = os.path.join(os.path.abspath(os.path.dirname(__file__)), 's1c88.csv')

op0s, op1s, op2s = [None] * 0x100, [None] * 0x100, [None] * 0x100
op0s[0xCE] = "inst_extended_ce"
op0s[0xCF] = "inst_extended_cf"

print ("typedef int (*InstructionCall)(Machine::State& cpu);")

CONDITIONS = {
    'C': 'cpu.reg.flag.c',
    'NC': '!cpu.reg.flag.c',
    'Z': 'cpu.reg.flag.z',
    'NZ': '!cpu.reg.flag.z',
    'V': 'cpu.reg.flag.v',
    'NV': '!cpu.reg.flag.v',
    'M': 'cpu.reg.flag.n',
    'P': '!cpu.reg.flag.n',
    'LT': 'cpu.reg.flag.n != cpu.reg.flag.v',
    'LE': '(cpu.reg.flag.n != cpu.reg.flag.v) || cpu.reg.flag.z',
    'GT': '(cpu.reg.flag.n == cpu.reg.flag.v) && !cpu.reg.flag.z',
    'GE': 'cpu.reg.flag.n == cpu.reg.flag.v',

    'F0': 'cpu.reg.flag.f0',
    'F1': 'cpu.reg.flag.f1',
    'F2': 'cpu.reg.flag.f2',
    'F3': 'cpu.reg.flag.f3',
    'NF0': '!cpu.reg.flag.f0',
    'NF1': '!cpu.reg.flag.f1',
    'NF2': '!cpu.reg.flag.f2',
    'NF3': '!cpu.reg.flag.f3',
}

ARGUMENTS = {
    'A': (8, False, False, 'a'),
    'B': (8, False, False, 'b'),
    'L': (8, False, False, 'l'),
    'H': (8, False, False, 'h'),
    'BR': (8, False, False, 'br'),
    'SC': (8, False, False, 'sc'),
    'EP': (8, False, False, 'ep'),
    'XP': (8, False, False, 'xp'),
    'YP': (8, False, False, 'yp'),
    'NB': (8, False, False, 'nb'),

    'BA': (16, False, False, 'ba'),
    'HL': (16, False, False, 'hl'),
    'IX': (16, False, False, 'ix'),
    'IY': (16, False, False, 'iy'),
    'SP': (16, False, False, 'sp'),
    'PC': (16, False, False, 'pc'),

    '#nn': (8, True, False, 'imm8'),
    'rr': (8, True, False, 'imm8'),
    '#mmnn': (16, True, False, 'imm16'),
    'qqrr': (16, True, False, 'imm16'),

    '[kk]': (16, True, True, 'vect'), # Special
    '[hhll]': (-1, True, True, 'ind16'),
    '[HL]': (-1, True, True, 'absHL'),
    '[IX]': (-1, True, True, 'absIX'),
    '[IY]': (-1, True, True, 'absIY'),
    '[BR:ll]': (-1, True, True, 'absBR'),
    '[SP+dd]': (-1, True, True, 'indDSP'),
    '[IX+dd]': (-1, True, True, 'indDIX'),
    '[IY+dd]': (-1, True, True, 'indDIY'),
    '[IX+L]': (-1, True, True, 'indIIX'),
    '[IY+L]': (-1, True, True, 'indIIY'),
}

OPERATIONS = {
    'INC': (8, 'ReadWrite'),
    'DEC': (8, 'ReadWrite'),
    'SLA': (8, 'ReadWrite'),
    'SLL': (8, 'ReadWrite'),
    'SRA': (8, 'ReadWrite'),
    'SRL': (8, 'ReadWrite'),
    'RL': (8, 'ReadWrite'),
    'RLC': (8, 'ReadWrite'),
    'RR': (8, 'ReadWrite'),
    'RRC': (8, 'ReadWrite'),
    'CPL': (8, 'ReadWrite'),
    'NEG': (8, 'ReadWrite'),

    'LD': (8, 'Write', 'Read'),
    'ADD': (8, 'ReadWrite', 'Read'),
    'ADC': (8, 'ReadWrite', 'Read'),
    'SUB': (8, 'ReadWrite', 'Read'),
    'SBC': (8, 'ReadWrite', 'Read'),
    'AND': (8, 'ReadWrite', 'Read'),
    'OR': (8, 'ReadWrite', 'Read'),
    'XOR': (8, 'ReadWrite', 'Read'),
    'CP': (8, 'Read', 'Read'),
    'BIT': (8, 'Read', 'Read'),

    'CALL': (16, 'Read'),
    'CARS': (8, 'Read'),
    'CARL': (8, 'Read'),
    'JRS': (8, 'Read'),
    'JRL': (8, 'Read'),
    'JP': (8, 'Read'),
    'INT': (8, 'Read'),

    'PUSH': (-1, 'Read'),
    'POP': (-1, 'Write'),
    'EX': (-1, 'ReadWrite', 'ReadWrite'),
    'SWAP': (8, 'ReadWrite')
}

def get_name(*args):
    return "inst_%s" % '_'.join([arg.lower() for arg in args if arg])

def format_arg(i, siz, mem, ind, nam):
    if mem:
        return "data%i" % i
    else:
        return "cpu.reg.%s" % nam

def format(cycles, op, *args):
    condition = None

    cycles, skipped = [int(c) for c in cycles.split(",") * 2][:2]

    if args[0] in CONDITIONS:
        condition, args = args[0], args[1:]
    
    try:
        ops = OPERATIONS[op]
        args = [ARGUMENTS[arg] for arg in args if arg]

        default_size, directions = ops[0], ops[1:]

        size = max(default_size, *[s for s, i, m, n in args])
        name = get_name(op, condition, *[n for s, i, m, n in args])

        print ("int %s(Machine::State& cpu) {" % name)

        for i, (siz, mem, ind, nam) in enumerate(args):
            if ind:
                print ("\tauto addr%i = calc_%s(cpu);" % (i, nam))
                if "Read" in directions[i]:
                    print ("\tuint%i_t data%i = cpu_read%s(cpu, addr%i);" % (size, i, size, i))
                else:
                    print ("\tuint%i_t data%i;" % (size, i))
            elif mem:
                print ("\tuint%i_t data%i = cpu_imm%i(cpu);" % (size, i, siz))


        if condition:
            print ("\tif (!(%s)) {" % CONDITIONS[condition])
            print ("\t\tcpu.reg.nb = cpu.reg.cb;")
            print ("\t\treturn %i;" % skipped)
            print ("\t}")

        print ("\top_%s%i(cpu, %s);" % (op.lower(), size, ', '.join([format_arg(i, *a) for i, a in enumerate(args)])));

        block = False
        for i, (siz, mem, ind, nam) in enumerate(args):
            if ind and "Write" in directions[i]:
                print ("\tcpu_write%s(cpu, data%i, addr%i);" % (size, i, i))
            if nam in ['sc', 'nb'] and "Write" in directions[i]:
                block = True

        if block:
            print ("\treturn %i + inst_advance(cpu); // Block IRQs" % cycles)
        else:
            print ("\treturn %i;" % cycles)
        print ("}\n")
        return name
    except:
        name = get_name(op, condition, *args)

        print ("int clock_%s(Machine::State& cpu) {" % name)
        print ("\t%s(cpu);" % name)
        print ("\treturn %i;" % cycles)
        print ("}\n")
        return "clock_%s" % name


with open(CSV_LOCATION, 'r') as csvfile:
    spamreader = csv.reader(csvfile)

    next(spamreader)

    for row in spamreader:
        code, cycles0, op0, arg0_1, arg0_2, cycles1, op1, arg1_1, arg1_2, cycles2, op2, arg2_1, arg2_2 = row
        code = int(code, 16)

        if not op0 in ['[EXPANSION]', 'undefined']:
        	op0s[code] = format(cycles0, op0, arg0_1, arg0_2)
        if op1 != 'undefined':
        	op1s[code] = format(cycles1, op1, arg1_1, arg1_2)
        if op2 != 'undefined':
        	op2s[code] = format(cycles2, op2, arg2_1, arg2_2)

for i, t in enumerate([op0s, op1s, op2s]):
    print ("static InstructionCall inst_table%i[] = {" % i)
    for inst in t:
        print ("\t%s," % (inst or "inst_undefined"))
    print ("};")
