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
			address: 0x9A
		};
	}

	renderArg() {
		return [];
	}

	render() {
		const disasm = new Disassembler(this.props.system);

		const lines = disasm.process(this.state.address, this.props.rowCount);

		return (
			<tbody>
				{
				lines.map(line =>
					<tr key={line.address}>
						<td>{toHex(this.props.system.translate(line.address), 6)}</td>
						<td>{line.op}</td>
						<td>{line.args.join(", ")}</td>
						<td>{line.data.map(v => <span> {toHex(v, 2)}</span>)}</td>
						<td>{line.comment}</td>
					</tr>)
				}
			</tbody>
		);
	}
}

class DisassemblyView extends Component {
	render() {
		return (
			<table className={classes['disasm']}>
				<Disassembly
					system={this.props.system}
					pc={0x9A}

					rowCount={800}
					/>
			</table>
			/*
			<AutoSizer className={classes['disasm']}>
				{({ height, width }) => (
						<Disassembly
							system={this.props.system}
							pc={0x9D}

							rowCount={height / 20}
							/>
					)
				}}
			</AutoSizer>
			*/
		);
	}
}

export default DisassemblyView;
