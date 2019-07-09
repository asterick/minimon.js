import React, { Component } from "react";

import classes from "./style.scss";
import SystemContext from "../context";

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

export default class Registers extends Component {
	static contextType = SystemContext;

	render() {
		const system = this.context;
		const sc = system.registers.sc;

		return (
			<div className={classes.registers}>
				<table>
					<tbody>
						<tr><td>BA</td><td>{toHex(system.registers.ba, 4)}</td><td>BR</td><td>{toHex(system.registers.br, 2)}</td></tr>
						<tr><td>HL</td><td>{toHex(system.registers.hl, 4)}</td><td>EP</td><td>{toHex(system.registers.ep, 2)}</td></tr>
						<tr><td>IX</td><td>{toHex(system.registers.ix, 4)}</td><td>XP</td><td>{toHex(system.registers.xp, 2)}</td></tr>
						<tr><td>IY</td><td>{toHex(system.registers.iy, 4)}</td><td>YP</td><td>{toHex(system.registers.yp, 2)}</td></tr>
						<tr><td>PC</td><td>{toHex(system.registers.pc, 4)}</td><td>CB</td><td>{toHex(system.registers.cb, 2)}</td></tr>
						<tr><td>SP</td><td>{toHex(system.registers.sp, 4)}</td><td>NB</td><td>{toHex(system.registers.nb, 2)}</td></tr>
					</tbody>
				</table>
				<div className={classes.flags}>
					<div className={classes.register}>
						<span className={classes.name}>SC</span>
						<span className={classes.value}>{toHex(sc, 2)}</span>
					</div>
					<div className={classes.register}>
						<span className={classes.name}>I</span>
						<span className={classes.value}>{sc >> 6}</span>
					</div>
					<div className={classes.register}>
						<span className={classes.name}>U</span>
						<div className={(sc & 0x20) ? classes.checked : classes.unchecked} />
					</div>
					<div className={classes.register}>
						<span className={classes.name}>D</span>
						<div className={(sc & 0x10) ? classes.checked : classes.unchecked} />
					</div>
					<div className={classes.register}>
						<span className={classes.name}>B</span>
						<div className={(sc & 0x08) ? classes.checked : classes.unchecked} />
					</div>
					<div className={classes.register}>
						<span className={classes.name}>V</span>
						<div className={(sc & 0x04) ? classes.checked : classes.unchecked} />
					</div>
					<div className={classes.register}>
						<span className={classes.name}>C</span>
						<div className={(sc & 0x02) ? classes.checked : classes.unchecked} />
					</div>
					<div className={classes.register}>
						<span className={classes.name}>Z</span>
						<div className={(sc & 0x01) ? classes.checked : classes.unchecked} />
					</div>
				</div>
			</div>
		);
	}
}

