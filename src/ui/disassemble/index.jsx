import React, { Component } from "react";

import classes from "./style.css";

import Disassembler from "../../system/disassemble.js";

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class Disassembly extends Component {
	constructor() {
		super();

		this.state = {
			address: 0
		};
	}

	renderArg() {
		return [];
	}

	render() {
		const disasm = new Disassembler(this.props.system);

		const lines = disasm.process(this.state.address, this.props.rowCount);

		if (this.props.follow_pc) {
			let match = this.props.system.registers.pc;

			for (let i = 0; i < lines.length; i++) {
				if (lines[i].address == this.props.system.registers.pc) {
					match = lines[Math.max(0, i - this.props.rowCount / 2) | 0].address;
				}
			}

			if (match !== this.state.address) {
				this.setState({ address: match });
			}
		}

		const { width, height } = this.props;

		return (
			<table  className={classes['disasm']}>
				<tbody>
					{
					lines.map(line =>
						<tr key={line.address} className={(this.props.system.registers.pc == line.address) ? classes['active'] : ''}>
							<td className={classes["address"]}>{toHex(this.props.system.translate(line.address), 6)}</td>
							<td>{line.data.map((v, i) => <span className={classes['byte-cell']} key={i}>{toHex(v, 2)}&nbsp;</span>)}</td>
							<td>{line.op}</td>
							<td>{line.args.map(s => <span>{s}</span>)}</td>
							<td>{line.comment || "; This is a comment"}</td>
						</tr>)
					}
				</tbody>
			</table>
		);
	}
}

Disassembly.defaultProps = {
	rowCount: 21,
	follow_pc: true
};

export default Disassembly;
