const fs = require("fs");
const path = require("path");

const { generate } = require("pegjs");

const parser = generate(fs.readFileSync(path.join(__dirname, "struct.pegjs"), "utf-8"), { cache: true });



function Struct(body, offset = 0) {
	const definitions = {};
	const fields = {};

	let size = 0;

	body.forEach(term => {
		switch (term.type) {
			case 'Union':
			case 'Struct':
			default:
			 	throw new Error(term.type);
		}
	})

	return { definitions, fields, size };
}

module.exports = {
	default: function(source) {
		const body = parser.parse(source)
		console.log(Struct(body));
		return "export default null";
	}
}
