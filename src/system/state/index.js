const { struct, union, sizeOf } = require("@thi.ng/unionstruct");

const CPUState = ["cpu", "union", [
	["registers", "struct", [
		["ba", "u16"],
		["hl", "u16"],
		["pc", "u16"],
		["sp", "u16"],
		["ix", "u16"],
		["iy", "u16"],
		["br", "u8"],
		["ep", "u8"],
		["xp", "u8"],
		["yp", "u8"],
		["cb", "u8"],
		["nb", "u8"],
		["sc", "u8"],
		["cc", "u8"],
	]],
]];

const MachineState = [
	CPUState
];

export default function Struct(buffer, offset) {
	return struct(MachineState, buffer, offset, false, true);
}
