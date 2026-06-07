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

import { useEffect, useRef, useState } from "react";

import classes from "./style.module.less";
import { useSystem, useSystemState } from "../context";
import { useElementSize } from "../use-element-size";
import { toHex } from "../util";

import { Disassembler } from "../../system/disassemble";

interface DisassemblyProps {
	follow_pc?: boolean;
}

export default function Disassembly({ follow_pc = true }: DisassemblyProps) {
	const system = useSystem();
	useSystemState();

	const [address, setAddress] = useState(0);

	const container = useRef<HTMLDivElement>(null);
	const { width, height } = useElementSize(container);

	// Keep the program counter centered in the visible window
	useEffect(() => {
		if (!follow_pc) return;

		const target = system.state.cpu.pc;
		const disasm = new Disassembler(system);
		const lines = disasm.process(address, 21);

		let next = target;

		for (let i = 0; i < lines.length; i++) {
			if (lines[i].address == target) {
				next = lines[Math.max(0, i - 21 / 2) | 0].address;
			}
		}

		if (next !== address) {
			setAddress(next);
		}
	});

	function onClick(target: number) {
		if (system.breakpoints.indexOf(target) >= 0) {
			system.breakpoints = system.breakpoints.filter((v) => v != target);
		} else {
			system.breakpoints.push(target);
		}

		system.update();
	}

	const target = system.state.cpu.pc;
	const rowCount = Math.floor(height / 20);

	const disasm = new Disassembler(system);
	const lines = (rowCount > 0) ? disasm.process(address, rowCount) : [];

	return (
		<div ref={container} className={classes.disasm} style={{width: '100%', height: '100%', overflow: 'hidden'}}>
			<table style={{width: `${width}px`, height: `${height}px`}}>
				<tbody>
					{
					lines.map(line =>
						<tr onClick={() => onClick(line.address)} key={line.address} className={(target == line.address) ? classes['active'] : ''}>
							<td className={classes["address"]}>{toHex(line.address, 6)}</td>
							<td>{line.data.map((v, i) => <span className={classes['byte-cell']} key={i}>{toHex(v, 2)}</span>)}</td>
							<td className={system.breakpoints.indexOf(line.address) >= 0 ? classes['breakpoint'] : ''}>{line.op}</td>
							<td>{line.args.map((s, i) => <span key={i}>{s}</span>)}</td>
						</tr>)
					}
				</tbody>
			</table>
		</div>
	);
}
