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
import { Toggle } from '@fluentui/react/lib/Toggle';
import { Stack } from '@fluentui/react/lib/Stack';
import { DefaultButton } from '@fluentui/react/lib/Button';

import { useSystem, useSystemState } from "../context";

const stackTokens = { childrenGap: 10 };

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
		<div style={{ padding: "10px" }}>
			<input ref={fileDialog} type="file" style={{display:'none'}} onChange={load} />
			<Stack tokens={stackTokens}>
				<Stack horizontal tokens={stackTokens}>
					<DefaultButton text="Reset" onClick={() => system.reset()} allowDisabledFocus />
					<DefaultButton text="Eject" onClick={() => system.eject()} allowDisabledFocus />
					<DefaultButton text="Step" onClick={() => system.step()} allowDisabledFocus />
					<DefaultButton text="Load" onClick={() => fileDialog.current?.click()} allowDisabledFocus />
				</Stack>

				<Toggle checked={system.running} inlineLabel onText="Running" offText="Stopped" onChange={(_e, checked) => {
					system.running = !!checked;
				}} />
				<Toggle checked={system.tracing} inlineLabel onText="Tracing on" offText="Tracing off" onChange={(_e, checked) => {
					system.tracing = !!checked;
				}} />
			</Stack>
		</div>
	);
}
