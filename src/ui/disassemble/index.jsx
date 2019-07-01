import React, { Component } from "react";

import classes from "./style.css";

import Disassembler from "../../system/disassemble.js";

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class Disassembly extends Component {
	constructor(props) {
		super();

		this.state = {
			address: props.target
		};
	}

	renderArg() {
		return [];
	}

	componentDidUpdate(props) {
		if (!this.props.follow_pc) {
			return ;
		}

		const { system, target, rowCount} = this.props;
		const disasm = new Disassembler(system);

		const lines = disasm.process(this.state.address, rowCount);

		let address = target;

		for (let i = 0; i < lines.length; i++) {
			if (lines[i].address == target) {
				address = lines[Math.max(0, i - rowCount / 2) | 0].address;
			}
		}

		if (address !== this.state.address) {
			this.setState({ address });
		}
	}

	render() {
		const { width, height, target } = this.props;

		const disasm = new Disassembler(this.props.system);
		const lines = disasm.process(this.state.address, this.props.rowCount);

		return (
			<table  className={classes['disasm']}>
				<tbody>
					{
					lines.map(line =>
						<tr key={line.address} className={(target == line.address) ? classes['active'] : ''}>
							<td className={classes["address"]}>{toHex(this.props.system.translate(line.address), 6)}</td>
							<td>{line.data.map((v, i) => <span className={classes['byte-cell']} key={i}>{toHex(v, 2)}&nbsp;</span>)}</td>
							<td>{line.op}</td>
							<td>{line.args.map((s, i) => <span key={i}>{s}</span>)}</td>
							<td>{line.comment}</td>
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
