const { struct, union, sizeOf } = require("@thi.ng/unionstruct");

const registerHeader = [
	["ba", "u16"],
	["hl", "u16"],
	["ix", "u16"],
	["iy", "u16"],
	["pc", "u16"],
	["sp", "u16"],
	["br", "u8"],
	["ep", "u8"],
	["xp", "u8"],
	["yp", "u8"],
	["cb", "u8"],
	["nb", "u8"],
	["sc", "u8"]
]

export default function Registers(ab, offset) {
	debugger ;
	return struct(registerHeader, ab, offset, false, true);
}
