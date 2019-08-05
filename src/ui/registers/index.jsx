/*
ISC License

Copyright (c) 2019, Bryon Vandiver

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

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
		const sc = system.state.cpu.registers.sc;
		const regs = system.state.cpu.registers;

		return (
			<div className={classes.registers}>
				<table>
					<tbody>
						<tr><td>BA</td><td>{toHex(regs.ba, 4)}</td><td>BR</td><td>{toHex(regs.br, 2)}</td></tr>
						<tr><td>HL</td><td>{toHex(regs.hl, 4)}</td><td>EP</td><td>{toHex(regs.ep, 2)}</td></tr>
						<tr><td>IX</td><td>{toHex(regs.ix, 4)}</td><td>XP</td><td>{toHex(regs.xp, 2)}</td></tr>
						<tr><td>IY</td><td>{toHex(regs.iy, 4)}</td><td>YP</td><td>{toHex(regs.yp, 2)}</td></tr>
						<tr><td>PC</td><td>{toHex(regs.pc, 4)}</td><td>CB</td><td>{toHex(regs.cb, 2)}</td></tr>
						<tr><td>SP</td><td>{toHex(regs.sp, 4)}</td><td>NB</td><td>{toHex(regs.nb, 2)}</td></tr>
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

