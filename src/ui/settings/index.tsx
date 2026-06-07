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

import { useRef } from "react";

import classes from "./style.module.less";
import { useSystem, useSystemState } from "../context";

interface ToggleProps {
	checked: boolean;
	onText: string;
	offText: string;
	onChange: (checked: boolean) => void;
}

function Toggle({ checked, onText, offText, onChange }: ToggleProps) {
	return (
		<label className={classes.toggle}>
			<input type="checkbox" checked={checked} onChange={(e) => onChange(e.target.checked)} />
			<span className={classes.track}><span className={classes.thumb} /></span>
			{checked ? onText : offText}
		</label>
	);
}

export default function Settings() {
	const system = useSystem();
	useSystemState();

	const fileDialog = useRef<HTMLInputElement>(null);

	async function load(e: React.ChangeEvent<HTMLInputElement>) {
		const file = e.target.files?.[0];
		if (!file) return;

		system.load(await file.arrayBuffer());
	}

	return (
		<div className={classes.settings}>
			<input ref={fileDialog} type="file" style={{display:'none'}} onChange={load} />
			<div className={classes.row}>
				<button onClick={() => system.reset()}>Reset</button>
				<button onClick={() => system.eject()}>Eject</button>
				<button onClick={() => system.step()}>Step</button>
				<button onClick={() => fileDialog.current?.click()}>Load</button>
			</div>

			<Toggle checked={system.running} onText="Running" offText="Stopped" onChange={(v) => {
				system.running = v;
			}} />
			<Toggle checked={system.tracing} onText="Tracing on" offText="Tracing off" onChange={(v) => {
				system.tracing = v;
			}} />
		</div>
	);
}
