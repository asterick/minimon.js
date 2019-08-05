const { struct, union, sizeOf } = require("@thi.ng/unionstruct");

const CPUState = ["cpu", "union", [
	["registers", "struct", [
		["sc", "u8"],
		["cc", "u8"],
		["_", "u16"],
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
	]],

	["flags", "struct", [
		["i", "u8", 2],
		["u", "u8", 1],
		["d", "u8", 1],
		["n", "u8", 1],
		["v", "u8", 1],
		["c", "u8", 1],
		["z", "u8", 1],
	]]
]];

const MachineState = [
	CPUState
];

export default function Struct(buffer, offset) {
	return struct(MachineState, buffer, offset, false, true);
}
