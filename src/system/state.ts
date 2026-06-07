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
 * Builds a live JavaScript view over the wasm core's Machine::State by
 * walking the StructDecl/FieldDecl reflection tables exposed through
 * get_description() (see core/wasm/main.cc). Scalar fields become
 * accessors over a DataView; arrays become typed-array windows directly
 * into wasm memory. The shape of the result is described by the
 * interfaces in ./machine-state.ts.
 */

const decoder = new TextDecoder("utf-8");

const TYPE_END = 0;
const TYPE_STRUCT = 1;
const TYPE_UINT8 = 2;
const TYPE_UINT16 = 3;
const TYPE_UINT32 = 4;
const TYPE_INT8 = 5;
const TYPE_INT16 = 6;
const TYPE_INT32 = 7;
const TYPE_BOOL = 8;

type ScalarType =
	| typeof TYPE_UINT8
	| typeof TYPE_UINT16
	| typeof TYPE_UINT32
	| typeof TYPE_INT8
	| typeof TYPE_INT16
	| typeof TYPE_INT32
	| typeof TYPE_BOOL;

const SIZES: Record<ScalarType, number> = {
	[TYPE_UINT8]: 1,
	[TYPE_UINT16]: 2,
	[TYPE_UINT32]: 4,
	[TYPE_INT8]: 1,
	[TYPE_INT16]: 2,
	[TYPE_INT32]: 4,
	[TYPE_BOOL]: 1
};

const GETTERS: Record<ScalarType, (dv: DataView, offset: number) => number> = {
	[TYPE_UINT8]: (dv, o) => dv.getUint8(o),
	[TYPE_UINT16]: (dv, o) => dv.getUint16(o, true),
	[TYPE_UINT32]: (dv, o) => dv.getUint32(o, true),
	[TYPE_INT8]: (dv, o) => dv.getInt8(o),
	[TYPE_INT16]: (dv, o) => dv.getInt16(o, true),
	[TYPE_INT32]: (dv, o) => dv.getInt32(o, true),
	[TYPE_BOOL]: (dv, o) => dv.getUint8(o)
};

const SETTERS: Record<ScalarType, (dv: DataView, offset: number, v: number) => void> = {
	[TYPE_UINT8]: (dv, o, v) => dv.setUint8(o, v),
	[TYPE_UINT16]: (dv, o, v) => dv.setUint16(o, v, true),
	[TYPE_UINT32]: (dv, o, v) => dv.setUint32(o, v, true),
	[TYPE_INT8]: (dv, o, v) => dv.setInt8(o, v),
	[TYPE_INT16]: (dv, o, v) => dv.setInt16(o, v, true),
	[TYPE_INT32]: (dv, o, v) => dv.setInt32(o, v, true),
	[TYPE_BOOL]: (dv, o, v) => dv.setUint8(o, v)
};

type TypedArray =
	| Uint8Array
	| Uint16Array
	| Uint32Array
	| Int8Array
	| Int16Array
	| Int32Array;

interface TypedArrayConstructor {
	new (buffer: ArrayBuffer, byteOffset: number, length: number): TypedArray;
}

const ARRAYTYPE: Record<ScalarType, TypedArrayConstructor> = {
	[TYPE_UINT8]: Uint8Array,
	[TYPE_UINT16]: Uint16Array,
	[TYPE_UINT32]: Uint32Array,
	[TYPE_INT8]: Int8Array,
	[TYPE_INT16]: Int16Array,
	[TYPE_INT32]: Int32Array,
	[TYPE_BOOL]: Uint8Array
};

function utf8(dv: DataView, offset: number): string | undefined {
	const address = dv.getUint32(offset, true);
	if (!address) return;
	let length = 0;
	while (dv.getUint8(address + length)) length++;
	return decoder.decode(dv.buffer.slice(address, address + length));
}

function sizeof(dv: DataView, offset: number): number[] | undefined {
	let address = dv.getUint32(offset, true);
	if (!address) return;

	const out = [];
	while (true) {
		const len = dv.getInt32(address, true);
		if (len < 0) break;
		out.push(len);
		address += 4;
	}

	return out;
}

function array(
	buffer: ArrayBuffer,
	offset: number,
	type: number,
	my_def: number,
	elements: number,
	...size: number[]
): unknown {
	const dv = new DataView(buffer);
	let stride = size.reduce((a, b) => a * b, 1);

	// Offset based on element size
	if (type == TYPE_STRUCT) {
		stride *= dv.getUint32(my_def, true);
	} else {
		stride *= SIZES[type as ScalarType];
	}

	// Return a sub array
	if (size.length > 0) {
		const [elements_inner, ...rest] = size;
		const out = new Array(elements);
		for (let i = 0; i < elements; i++) {
			out[i] = array(buffer, offset + stride * i, type, my_def, elements_inner, ...rest);
		}
		return out;
	} else if (type == TYPE_STRUCT) {
		const out = new Array(elements);
		for (let i = 0; i < elements; i++) {
			out[i] = struct(buffer, my_def, offset + stride * i);
		}
		return out;
	}

	return new ARRAYTYPE[type as ScalarType](buffer, offset, elements);
}

export default function struct(buffer: ArrayBuffer, my_def: number, offset: number): Record<string, unknown> {
	const dv = new DataView(buffer);
	const out: Record<string, unknown> = {};

	let fields = dv.getUint32(my_def + 4, true);

	for (;;) {
		const type = dv.getUint32(fields, true);

		if (type == TYPE_END) break;

		const name = utf8(dv, fields + 4)!;

		const target = dv.getUint32(fields + 8, true);
		const def = dv.getUint32(fields + 12, true);
		const size = sizeof(dv, fields + 16);

		if (size) {
			const [elements, ...rest] = size;
			Object.defineProperty(out, name, {
				enumerable: true,
				value: array(buffer, target + offset, type, def, elements, ...rest)
			});
		} else if (type == TYPE_STRUCT) {
			Object.defineProperty(out, name, {
				enumerable: true,
				value: struct(buffer, def, target + offset)
			});
		} else {
			Object.defineProperty(out, name, {
				enumerable: true,
				get: () => GETTERS[type as ScalarType](dv, target + offset),
				set: (v: number) => SETTERS[type as ScalarType](dv, target + offset, v)
			});
		}

		fields += 20;
	}

	return out;
}
