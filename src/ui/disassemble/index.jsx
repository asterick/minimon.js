import React, { Component } from "react";
import { AutoSizer } from 'react-virtualized';

import classes from "./style.scss";
import SystemContext from "../context";

import { Disassembler } from "../../system/disassemble.js";

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class Disassembly extends Component {
	static contextType = SystemContext;

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

		const { target } = this.props;
		const system = this.context;
		const disasm = new Disassembler(system);

		const lines = disasm.process(this.state.address, 21);

		let address = target;

		for (let i = 0; i < lines.length; i++) {
			if (lines[i].address == target) {
				address = lines[Math.max(0, i - 21 / 2) | 0].address;
			}
		}

		if (address !== this.state.address) {
			this.setState({ address });
		}
	}

	render() {
		const { target } = this.props;

		return (
			<div className={classes.disasm}>
				<AutoSizer>
					{({ height, width }) => {
						const rowCount = height / 20;

						const system = this.context;
						const disasm = new Disassembler(system);
						const lines = disasm.process(this.state.address, rowCount);

						return <table style={{width: `${width}px`, height: `${height}px`}}>
							<tbody>
								{
								lines.map(line =>
									<tr key={line.address} className={(target == line.address) ? classes['active'] : ''}>
										<td className={classes["address"]}>{toHex(system.translate(line.address), 6)}</td>
										<td>{line.data.map((v, i) => <span className={classes['byte-cell']} key={i}>{toHex(v, 2)}</span>)}</td>
										<td>{line.op}</td>
										<td>{line.args.map((s, i) => <span key={i}>{s}</span>)}</td>
										<td>{line.comment}</td>
									</tr>)
								}
							</tbody>
						</table>
					}}
				</AutoSizer>
			</div>
		);
	}
}

Disassembly.defaultProps = {
	follow_pc: true
};

export default Disassembly;
