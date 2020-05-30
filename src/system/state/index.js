let decoder =  new TextDecoder("utf-8");

const TYPE_END = 0;
const TYPE_STRUCT = 1;
const TYPE_UINT8 = 2;
const TYPE_UINT16 = 3;
const TYPE_UINT32 = 4;
const TYPE_INT8 = 5;
const TYPE_INT16 = 6;
const TYPE_INT32 = 7;
const TYPE_BOOL = 8;

const SIZES = {
	[TYPE_UINT8]: 1,
	[TYPE_UINT16]: 2,
	[TYPE_UINT32]: 4,
	[TYPE_INT8]: 1,
	[TYPE_INT16]: 2,
	[TYPE_INT32]: 4,
	[TYPE_BOOL]: 1
};

const GETTERS = {
	[TYPE_UINT8]: 'getUint8',
	[TYPE_UINT16]: 'getUint16',
	[TYPE_UINT32]: 'getUint32',
	[TYPE_INT8]: 'getInt8',
	[TYPE_INT16]: 'getInt16',
	[TYPE_INT32]: 'getInt32',
	[TYPE_BOOL]: 'getUint8'
}

const SETTERS = {
	[TYPE_UINT8]: 'setUint8',
	[TYPE_UINT16]: 'setUint16',
	[TYPE_UINT32]: 'setUint32',
	[TYPE_INT8]: 'setInt8',
	[TYPE_INT16]: 'setInt16',
	[TYPE_INT32]: 'setInt32',
	[TYPE_BOOL]: 'setUint8'
}

const ARRAYTYPE = {
	[TYPE_UINT8]: Uint8Array,
	[TYPE_UINT16]: Uint16Array,
	[TYPE_UINT32]: Uint32Array,
	[TYPE_INT8]: Int8Array,
	[TYPE_INT16]: Int16Array,
	[TYPE_INT32]: Int32Array,
	[TYPE_BOOL]: Uint8Array
}

function utf8(dv, offset) {
	let address = dv.getUint32(offset, true);
	if (!address) return ;
	for (var length = 0; dv.getUint8(address+length); length++) ;
	return decoder.decode(dv.buffer.slice(address, address+length));
}

function sizeof(dv, offset) {
	let address = dv.getUint32(offset, true);
	if (!address) return ;

	let out = [];
	while (true) {
		let len = dv.getInt32(address, true);
		if (len < 0) break ;
		out.push(len);
		address += 4;
	}

	return out;
}

function array(buffer, offset, type, my_def, elements, ... size) {
	let dv = new DataView(buffer);
	let stride = size.reduce((a,b) => size, 1);

	// Offset based on element size
	if (type == TYPE_STRUCT) {
		stride *= dv.getUint32(my_def, true);
	} else {
		stride *= SIZES[type];
	}

	// Return a sub array
	if (size.length > 0) {
		let out = new Array(elements);
		for (let i = 0; i < elements; i++) {
			out[i] = array(buffer, offset + (stride*i), type, my_def, ... size);
		}
		return out;
	} else if (type == TYPE_STRUCT) {
		let out = new Array(elements);
		for (let i = 0; i < elements; i++) {
			out[i] = struct(buffer, my_def, offset + (stride*i))
		}
		return out;
	}

	return new (ARRAYTYPE[type])(buffer, offset, elements);
}

export default function struct(buffer, my_def, offset) {
	let dv = new DataView(buffer);
	let out = {};
	
	const size = dv.getUint32(my_def, true);
	let fields = dv.getUint32(my_def + 4, true);
	
	for (;;) {
		let type = dv.getUint32(fields, true);

		if (type == TYPE_END) break ;
		
		let name = utf8(dv, fields + 4);

		let target = dv.getUint32(fields + 8, true);
		let def = dv.getUint32(fields + 12, true);
		let size = sizeof(dv, fields+16);

		if (size) {
			Object.defineProperty(out, name, {
				enumerable: true,
				value: array(buffer, target+offset, type, def, ... size)
			});
		} else if (type == TYPE_STRUCT) {
			Object.defineProperty(out, name, {
				enumerable: true,
				value: struct(buffer, def, target+offset)
			})
		} else {
			Object.defineProperty(out, name, {
				enumerable: true,
				get: () => dv[GETTERS[type]](target+offset, true),
				set: (v) => dv[SETTERS[type]](target+offset, v, true)
			});
		}

		fields += 20;
	}

	return out ;
}
