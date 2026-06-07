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

import { useRef, useState } from "react";
import { useVirtualizer } from '@tanstack/react-virtual';

import classes from "./style.module.less";
import { useSystem, useSystemState } from "../context";
import { useElementSize } from "../use-element-size";
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
	useSystemState();

	const [measurements, setMeasurements] = useState<Measurements | null>(null);
	const scroller = useRef<HTMLDivElement>(null);
	const { width } = useElementSize(scroller);

	let memory: Uint8Array;

	switch (memoryName) {
		case 'ram':
			memory = system.state.ram;
			break;
		case 'eeprom':
			memory = system.state.gpio.eeprom.data;
			break;
	}

	const rowHeight = measurements?.rowHeight ?? 20;
	const bytesPerRow = measurements
		? Math.floor((width - measurements.baseWidth) / measurements.elementWidth) - 2
		: 0;
	const rowCount = (bytesPerRow > 0) ? Math.ceil(memory.length / bytesPerRow) : 0;

	const virtualizer = useVirtualizer({
		count: rowCount,
		getScrollElement: () => scroller.current,
		estimateSize: () => rowHeight,
	});

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

	/* Measure row / child sizes */
	if (!measurements) {
		return <div ref={scroller} className={classes.memory} style={{ width: '100%', height: '100%' }}>
			<div style={{position: 'absolute', visibility: "hidden"}} ref={measure}>
				<div className="one-element" style={{display: 'inline-block'}}>
					<span className={classes.address}>{toHex(0, 6)}</span>
					<span className={classes.byteCell}>00</span>
				</div>
				<div className="two-element" style={{display: 'inline-block'}}>
					<span className={classes.address}>{toHex(0, 6)}</span>
					<span className={classes.byteCell}>00</span>
					<span className={classes.byteCell}>00</span>
				</div>
			</div>
		</div>;
	}

	return (
		<div ref={scroller} className={classes.memory} style={{ width: '100%', height: '100%', overflowY: 'auto' }}>
			<div style={{ height: `${virtualizer.getTotalSize()}px`, position: 'relative' }}>
				{virtualizer.getVirtualItems().map((row) => {
					const address = row.index * bytesPerRow;
					const data = [];

					for (let i = 0, a = address; i < bytesPerRow && a < memory.length; i++, a++)
						data.push(<span key={a} className={classes.byteCell}>{toHex(memory[a], 2)}</span>);

					return (
						<div
							className={classes.dataRow}
							key={row.key}
							style={{
								position: 'absolute',
								top: 0,
								left: 0,
								width: '100%',
								height: `${row.size}px`,
								transform: `translateY(${row.start}px)`
							}}>
							<span className={classes.address}>{toHex(address + baseAddress, 6)}</span>
							{ data }
						</div>
					);
				})}
			</div>
		</div>
	);
}
