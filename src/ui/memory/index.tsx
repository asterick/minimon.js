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
import { AutoSizer, List, type ListRowProps } from 'react-virtualized';

import classes from "./style.module.less";
import { useSystem, useSystemState } from "../context";
import { toHex } from "../util";

interface Measurements {
	baseWidth: number;
	elementWidth: number;
	rowHeight: number;
}

interface MemoryProps {
	baseAddress: number;
	memory: 'ram' | 'eeprom';
}

export default function Memory({ baseAddress, memory: memoryName }: MemoryProps) {
	const system = useSystem();
	const version = useSystemState();

	const [measurements, setMeasurements] = useState<Measurements | null>(null);
	const list = useRef<List>(null);

	// The memory array is mutated in place by the emulation core, so the
	// virtualized rows must be told to redraw whenever state changes
	useEffect(() => {
		list.current?.forceUpdateGrid();
	}, [version]);

	let memory: Uint8Array;

	switch (memoryName) {
		case 'ram':
			memory = system.state.ram;
			break;
		case 'eeprom':
			memory = system.state.gpio.eeprom.data;
			break;
	}

	function measure(r: HTMLDivElement | null) {
		if (!r) return;

		const one = r.querySelector<HTMLElement>(".one-element")!;
		const two = r.querySelector<HTMLElement>(".two-element")!;

		const elementWidth = two.offsetWidth - one.offsetWidth;
		const rowHeight = one.offsetHeight;
		const baseWidth = one.offsetWidth - elementWidth;

		if (elementWidth != measurements?.elementWidth ||
			rowHeight != measurements?.rowHeight ||
			baseWidth != measurements?.baseWidth
			) {
			setMeasurements({
				baseWidth,
				elementWidth,
				rowHeight
			});
		}
	}

	return <AutoSizer>
		{({width, height}) => {
			/* Measure row / child sizes */
			if (!measurements) {
				return <div style={{position: 'absolute', visibility: "hidden"}} ref={measure}>
					<div className="one-element" style={{display: 'inline-block'}}>
						<span className={classes.address}>{toHex(0, 6)}</span>
						<span className={classes.byteCell}>00</span>
					</div>
					<div className="two-element" style={{display: 'inline-block'}}>
						<span className={classes.address}>{toHex(0, 6)}</span>
						<span className={classes.byteCell}>00</span>
						<span className={classes.byteCell}>00</span>
					</div>
				</div>;
			}

			const { baseWidth, elementWidth, rowHeight } = measurements;

			const bytesPerRow = Math.floor((width - baseWidth) / elementWidth) - 2;
			if (bytesPerRow <= 0) return null;

			function rowRenderer({ key, style, index }: ListRowProps) {
				const address = index * bytesPerRow;
				const data = [];

				for (let i = 0, a = address; i < bytesPerRow && a < memory.length; i++, a++)
					data.push(<span key={a} className={classes.byteCell}>{toHex(memory[a], 2)}</span>);

				return (
					<div className={classes.dataRow} key={key} style={style}>
						<span className={classes.address}>{toHex(address + baseAddress, 6)}</span>
						{ data }
					</div>
				)
			}

			return (
				<List
					className={classes.memory}
					ref={list}

					width={width}
					height={height}
					rowCount={Math.ceil(memory.length / bytesPerRow)}
					rowHeight={rowHeight}
					rowRenderer={rowRenderer}
					/>
			);
		}}
	</AutoSizer>
}
