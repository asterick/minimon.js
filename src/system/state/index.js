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

function utf8(bytes, offset) {
	let length = 0;

	while (bytes[offset+length]) length++;

	return decoder.decode(bytes.subarray(offset, offset+length));
}

export default function struct(buffer, offset) {
	let dv = new DataView(buffer);
	let bytes = new Uint8Array(buffer);
	let out = {};

	for (;; offset += 16) {
		let type = dv.getUint32(offset, true);

		console.log(type)

		if (type == TYPE_END) break ;

		let name = dv.getUint32(offset + 4, true);
		let data = dv.getUint32(offset + 8, true);
		let getter, setter;

		switch (dv.getUint32(offset, true)) {
		case TYPE_STRUCT:
			out[utf8(bytes, name)] = struct(buffer, data);
			continue ;
		case TYPE_UINT8:
			getter = () => dv.getUint8(data);
			setter = (v) => dv.setUint8(data, v);
			break ;
		case TYPE_BOOL:
			getter = () => dv.getUint8(data) != 0;
			setter = (v) => dv.setUint8(data, v ? 1 : 0);
			break ;
		case TYPE_UINT16:
			getter = () => dv.getUint16(data, true);
			setter = (v) => dv.setUint16(data, v, true);
			break ;
		case TYPE_UINT32:
			getter = () => dv.getUint32(data, true);
			setter = (v) => dv.setUint32(data, v, true);
			break ;
		case TYPE_INT8:
			getter = () => dv.getInt8(data, true);
			setter = (v) => dv.setInt8(data, v, true);
			break ;
		case TYPE_INT16:
			getter = () => dv.getInt16(data, true);
			setter = (v) => dv.setInt16(data, v, true);
			break ;
		case TYPE_INT32:
			getter = () => dv.getInt32(data, true);
			setter = (v) => dv.setInt32(data, v, true);
			break ;
		default:
			throw new Error(`Unhandled property type ${dv.getUint32(offset, true)}`);
			break ;
		}

		Object.defineProperty(out, utf8(bytes, name), {
			enumerable: true,
			get: getter,
			set: setter
		});
	}

	return out ;
}
