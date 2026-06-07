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

import { useEffect, useMemo, useRef, useState } from "react";
import { Button, ButtonGroup, HTMLSelect, InputGroup, Switch } from "@blueprintjs/core";
import { useVirtualizer } from '@tanstack/react-virtual';

import classes from "./style.module.less";
import { useSystem, useSystemState } from "../context";
import { DocumentCache, type DocumentLine, type DebuggerDocument } from "../../system/debugger-document";
import { toHex } from "../util";

const ROW_HEIGHT = 20;
const HEX_ROW = 16;

// 2MB of cartridge in 32K windows
const BANKS = Array.from({ length: 64 }, (_, i) => ({
	value: i,
	label: `Bank ${toHex(i, 2)}`,
}));

interface RowProps {
	document: DebuggerDocument;
	line: DocumentLine;
	atPC: boolean;
	hasBreakpoint: boolean;
	onToggleBreakpoint: (line: DocumentLine) => void;
}

function Row({ document, line, atPC, hasBreakpoint, onToggleBreakpoint }: RowProps) {
	const cells = [];

	if (line.kind === 'code') {
		const bytes = [];
		for (let i = 0; i < line.length; i++) {
			const value = document.byteAt(line.address + i);
			bytes.push(<span key={i}>{value !== null ? toHex(value, 2) : '--'}</span>);
		}

		cells.push(
			<span key="b" className={classes.bytes}>{bytes}</span>,
			<span key="o" className={classes.op}>{line.op}</span>,
			<span key="a" className={classes.args}>{line.args?.join(", ")}</span>,
		);
	} else {
		const bytes = [];
		for (let i = 0; i < line.length; i++) {
			const value = document.byteAt(line.address + i);
			bytes.push(<span key={i}>{value !== null ? toHex(value, 2) : '--'}</span>);
		}

		cells.push(
			<span key="h" className={`${classes.hex} ${line.kind === 'unknown' ? classes.unknown : ''}`}>
				{bytes}
			</span>
		);
	}

	return (
		<div
			className={`${classes.line} ${atPC ? classes.pc : ''}`}
			onDoubleClick={() => onToggleBreakpoint(line)}
		>
			<span className={classes.gutter}>
				<span className={classes.marker}>{hasBreakpoint ? '●' : ''}</span>
				<span className={classes.address}>{toHex(line.address, 4)}</span>
			</span>
			{cells}
		</div>
	);
}

export default function Debugger() {
	const system = useSystem();
	const version = useSystemState();

	const [followPC, setFollowPC] = useState(true);
	const [selectedBank, setSelectedBank] = useState(1);

	const cache = useMemo(() => new DocumentCache(), []);
	const scroller = useRef<HTMLDivElement>(null);

	const running = system.running;
	const pc = system.state.cpu.pc;
	const cb = system.state.cpu.cb;

	// Following the PC, the upper window tracks the live CB bank and the
	// dropdown is a passive indicator; unfollowed, the dropdown steers
	const bank = followPC ? cb : selectedBank;
	const doc = cache.get(system, bank, version);

	const virtualizer = useVirtualizer({
		count: doc.lines.length,
		getScrollElement: () => scroller.current,
		estimateSize: () => ROW_HEIGHT,
	});

	// The PC row only exists in this view if the upper window shows the
	// bank the PC executes from
	const pcVisible = (pc & 0x8000) === 0 || bank === cb;
	const pcLine = pcVisible ? doc.lineAt(pc) : -1;

	useEffect(() => {
		if (followPC && pcLine >= 0) {
			virtualizer.scrollToIndex(pcLine, { align: 'center' });
		}
	}, [followPC, pcLine, virtualizer]);

	function goTo(text: string) {
		const address = parseInt(text.trim().replace(/^0x/i, "").replace(/h$/i, ""), 16);
		if (isNaN(address) || address < 0 || address > 0xFFFF) return;

		// A manual jump means the user wants to look around
		setFollowPC(false);
		// Rows are fixed-height, so the offset is exact (scrollToIndex
		// works from estimates and can land a row short)
		virtualizer.scrollToOffset(doc.lineAt(address) * ROW_HEIGHT);
	}

	// A line carries a marker if any of its bytes has a breakpoint —
	// notably, halting at a breakpoint prevents that byte from being
	// classified as code, so the marker must show on hex rows too
	function lineBreakpoints(line: DocumentLine): number[] {
		const hits = [];
		for (let i = 0; i < line.length; i++) {
			if (system.breakpoints.has(line.physical + i)) hits.push(line.physical + i);
		}
		return hits;
	}

	function toggleBreakpoint(line: DocumentLine) {
		const existing = lineBreakpoints(line);

		if (existing.length) {
			for (const address of existing) system.breakpoints.delete(address);
		} else if (line.kind === 'code') {
			system.breakpoints.add(line.physical);
		} else {
			return;
		}

		system.update();
	}

	return (
		<div className={classes.debugger}>
			<div className={classes.toolbar}>
				<ButtonGroup>
					<Button
						icon={running ? "stop" : "play"}
						intent={running ? "danger" : "success"}
						title={running ? "Stop" : "Run"}
						onClick={() => { system.running = !running; }}
					/>
					<Button
						icon="step-forward"
						title="Step"
						disabled={running}
						onClick={() => system.step()}
					/>
				</ButtonGroup>

				<HTMLSelect
					options={BANKS}
					value={bank}
					disabled={followPC}
					onChange={(e) => setSelectedBank(Number(e.currentTarget.value))}
				/>

				<Switch
					checked={followPC}
					label="Follow PC"
					className={classes.follow}
					onChange={(e) => setFollowPC(e.currentTarget.checked)}
				/>

				<InputGroup
					placeholder="Go to…"
					className={classes.goto}
					onKeyDown={(e) => {
						if (e.key === 'Enter') goTo(e.currentTarget.value);
					}}
				/>
			</div>

			<div ref={scroller} className={classes.listing}>
				<div style={{ height: `${virtualizer.getTotalSize()}px`, position: 'relative' }}>
					{virtualizer.getVirtualItems().map((row) => {
						const line = doc.lines[row.index];

						return (
							<div
								key={row.key}
								style={{
									position: 'absolute',
									top: 0,
									left: 0,
									width: '100%',
									height: `${row.size}px`,
									transform: `translateY(${row.start}px)`,
								}}
							>
								<Row
									document={doc}
									line={line}
									atPC={row.index === pcLine}
									hasBreakpoint={lineBreakpoints(line).length > 0}
									onToggleBreakpoint={toggleBreakpoint}
								/>
							</div>
						);
					})}
				</div>
			</div>
		</div>
	);
}
