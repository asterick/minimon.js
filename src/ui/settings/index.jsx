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
import { Toggle } from 'office-ui-fabric-react/lib/Toggle';
import { Stack } from 'office-ui-fabric-react/lib/Stack';
import { DefaultButton } from 'office-ui-fabric-react';

import SystemContext from "../context";

const stackTokens = { childrenGap: 10 };

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

export default class Registers extends Component {
	static contextType = SystemContext;

	constructor(...props) {
		super(...props);
		this.fileDialog = React.createRef();
	}

	async load(e) {
		const file = e.target.files[0];
		this.context.system.load(await file.arrayBuffer());
	}

	render() {
		const system = this.context.system;

		return (
			<div style={{ padding: "10px" }}>
				<input ref={this.fileDialog} type="file" style={{display:'none'}} onChange={(e) => this.load(e)} />
				<Stack tokens={stackTokens}>
					<Stack horizontal tokens={stackTokens}>
						<DefaultButton text="Reset" onClick={() => system.reset()} allowDisabledFocus />
						<DefaultButton text="Eject" onClick={() => system.eject()} allowDisabledFocus />
						<DefaultButton text="Step" onClick={() => system.step()} allowDisabledFocus />
						<DefaultButton text="Load" onClick={() => this.fileDialog.current.click()} allowDisabledFocus />
					</Stack>

					<Toggle checked={system.running}inlineLabel onText="Running" offText="Stopped" onChange={(e, checked) => {
						system.running = checked;
					}} />
					<Toggle checked={system.tracing} inlineLabel onText="Tracing on" offText="Tracing off" onChange={(e, checked) => {
						system.tracing = checked;
					}} />
				</Stack>
			</div>
		);
	}
}
