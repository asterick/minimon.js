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

import SystemContext from "../context";

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

export default class Registers extends Component {
	static contextType = SystemContext;

	render() {
		const system = this.context.system;

		return (
			<div>
                <button onClick={() => system.eject()}>Eject</button>
                <button onClick={() => system.reset()}>Reset</button>
                <button onClick={() => system.step()}>Step</button>
                <button onClick={() => {system.running = !system.running}}>{system.running ? "Stop" : "Start"}</button>
                <button onClick={() => {system.tracing = !system.tracing}}>{system.tracing ? "Tracing" : "Not Tracing"}</button>
			</div>
		);
	}
}

