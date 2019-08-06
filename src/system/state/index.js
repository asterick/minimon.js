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

function utf8(bytes, offset) {
	let length = 0;

	while (bytes[offset+length]) length++;

	return decoder.decode(bytes.subarray(offset, offset+length));
}

function proxy(dv, type, offset, entries, ... rest) {
	const stride = rest.reduce((a, b) => a * b, SIZES[type]);

	return new Proxy({}, {
		get: function(_, index) {
			if (index < 0 || index >= entries) return null;

			if (rest.length > 0) {
				return proxy(dv, type, offset + index * stride, ... rest);
			} else {
				return dv[GETTERS[type]](offset + index * stride, true);
			}
		},
		set: function(_, index, value) {
			if (rest.length > 0) {
				throw new Error("Cannot assign array row")
			} else {
				if (index < 0 || index >= entries) return ;

				return dv[SETTERS[type]](offset + index * stride, value, true);
			}
		}
	})
}

export default function struct(buffer, offset) {
	let dv = new DataView(buffer);
	let bytes = new Uint8Array(buffer);
	let out = {};

	for (;; offset += 16) {
		let type = dv.getUint32(offset, true);

		if (type == TYPE_END) break ;

		let name = utf8(bytes, dv.getUint32(offset + 4, true));
		let data = dv.getUint32(offset + 8, true);
		let size = dv.getUint32(offset + 12, true);
		let getter, setter;

		if (size) {
			let topo = [];

			for (;; size += 4) {
				let dim = dv.getInt32(size, true);
				if (dim <= 0) break ;
				topo.push(dim);
			}

			out[name] = proxy(dv, type, data, ... topo);
		} else if (type == TYPE_STRUCT) {
			out[name] = struct(buffer, data);
		} else {
			Object.defineProperty(out, name, {
				enumerable: true,
				get: () => dv[GETTERS[type]](data, true),
				set: (v) => dv[SETTERS[type]](data, v, true)
			});
		}
	}

	return out ;
}
