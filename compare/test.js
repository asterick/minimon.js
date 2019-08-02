const fs = require('fs');

function process_text(data) {
	const exp = /case 0x(?<code>[0-9A-F]{2}):\s+\/\/\s+(?<op>\S+)(\s+(?<arg1>[^\s,]+)(\s*,\s*(?<arg2>\S*))?)?/i;
	const output = [];


	data.split("\n").forEach((l) => {
		const match = exp.exec(l);

		if (!match) return ;

		const { code, op, arg1, arg2 } = match.groups;

		output[parseInt(code, 16)] = { op, arg1, arg2 };
	});

	return output
}

function translate_op(op) {
	switch (op) {
	case 'movb': case 'movw': return 'LD';
	default: return op;
	}
}

function translate_arg(arg) {
	switch ((arg || '').toUpperCase()) {
	case '': return '';

	case 'NN':
	case 'N': return 'BR';
	case 'A': return 'A';
	case 'B': return 'B';
	case 'L': return 'L';
	case 'H': return 'H';
	case 'BA': return 'BA';
	case 'HL': return 'HL';
	case 'X1': return 'IX';
	case 'X2': return 'IY';
	case 'X': return 'IX';
	case 'Y': return 'IY';
	case 'SP': return 'SP';
	case 'PC': return 'PC';
	case 'U': return 'NB';
	case 'V': return 'CB';

	case '0': return 'F0';
	case '1': return 'F1';
	case '2': return 'F2';
	case '3': return 'F3';

	case '[HL]': return '[HL]';
	case '[X1]': return '[IX]';
	case '[X2]': return '[IY]';

	case 'FLAGS': return 'SC';
	case '[NN+%(I8)S]': return '[BR:ll]';
	case '[%(I16)S]': return '[hhll]';
	
	case '[X1+L]': return '[IX+L]';
	case '[X2+L]': return '[IY+L]';
	case '[X1+%(I8)S]': return '[IX+dd]';
	case '[X2+%(I8)S]': return '[IY+dd]';

	case '%(I16)S': return '#mmnn'
	case '%(B8)S': case '%(I8)S00': case '%(I8)S': return '#nn'
	case '%(D8)S': case '%(ADDR)S': return 'rr';
	case '%(IRQ)S': return '[kk]';
	case '[SP%(S8)S]': return '[SP+dd]';
	default: return `crud ${arg}`
	}
}

function translate(row) {
	if (!row) return ['undefined', '', '']

	const { op, arg1, arg2 } = row;
	return [ op || 'undefined', translate_arg(arg1), translate_arg(arg2) ];
}

function merge(... rows) {
	let out = [];

	for (let i = 0; i < rows.length; i++) {
		const data = process_text(fs.readFileSync(rows[i], 'utf8'));

		for (let l = 0; l < 0x100; l++) {
			let row = [];

			out.push(translate(data[l]));
		}
	}

	return out;
}

function csv(fn) {
	const rows = fs.readFileSync(fn, 'utf8').split(/\r?\n/);
	const out = [];

	for (let i = 1; i <= 0x100; i++) {
		const row = rows[i].split(",");
		out.push(row.slice(2,5));
	}

	for (let i = 1; i <= 0x100; i++) {
		const row = rows[i].split(",");
		out.push(row.slice(6,9));
	}

	for (let i = 1; i <= 0x100; i++) {
		const row = rows[i].split(",");
		out.push(row.slice(10,13));
	}

	return out;
}

function different(a, b) {
	return a.some((iff, i) => (iff != b[i]));
}

const minimon = merge("base.txt", "ce.txt", "cf.txt");
const newmini = csv("s1c88.csv");

for (let i = 0; i < minimon.length; i++) {
	if (!different(minimon[i], newmini[i])) continue ;

	console.log([i.toString(16)].concat(minimon[i], newmini[i]).join('\t'))
}
