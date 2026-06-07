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
import { Button, ButtonGroup, Switch } from "@blueprintjs/core";

import classes from "./style.module.less";
import { useSystem, useSystemState } from "../context";

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
			<ButtonGroup>
				<Button icon="reset" text="Reset" onClick={() => system.reset()} />
				<Button icon="eject" text="Eject" onClick={() => system.eject()} />
				<Button icon="step-forward" text="Step" onClick={() => system.step()} />
				<Button icon="folder-open" text="Load" onClick={() => fileDialog.current?.click()} />
			</ButtonGroup>

			<Switch checked={system.running} innerLabel="stopped" innerLabelChecked="running" label="Execution" onChange={(e) => {
				system.running = e.currentTarget.checked;
			}} />
		</div>
	);
}
