import React, { Component } from "react";
import { AutoSizer, List } from 'react-virtualized';

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
		let match = this.props.system.pc;

		for (let i = 0; i < lines.length; i++) {
			if (lines[i].address == this.props.system.pc) {
				match = lines[Math.max(0, i - this.props.rowCount / 2) | 0].address;
			}
		}

		if (match !== this.state.address) {
			this.setState({ address: match })
		}

		return (
			<table className={classes['disasm']}>
				<tbody>
					{
					lines.map(line =>
						<tr key={line.address} className={(this.props.system.pc == line.address) ? classes['active'] : ''}>
							<td className={classes["address"]}>{toHex(this.props.system.translate(line.address), 6)}</td>
							<td>{line.op}</td>
							<td>{line.args.join(", ")}</td>
							<td>{line.data.map((v, i) => <span className={classes['byte-cell']} key={i}>{toHex(v, 2)}&nbsp;</span>)}</td>
							<td>{line.comment}</td>
						</tr>)
					}
				</tbody>
			</table>
		);
	}
}

class DisassemblyView extends Component {
	render() {
		return (
			<AutoSizer>
				{({ height, width }) => 
					(
					<Disassembly
						system={this.props.system}
						pc={0x9A}

						rowCount={height / 20}
						/>
					)
				}
			</AutoSizer>
		);
	}
}

export default DisassemblyView;
