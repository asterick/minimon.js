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

import style from "./style.scss";
import SystemContext from "../context";

const MenuItem = (props) => {
	const { title } = props;
	return <span className={style.menuitem} {... props}>{title}</span>
}

const DropDown = ({ title, children }) => 
	<div className={style.dropdown}>
		<span className={style.menuitem}>{title}</span>
		<div className={style.dropchildren}>{ children }</div>
	</div>;

const Seperator = () => <div className={style.seperator}></div>

export default class MenuBar extends Component {
	static contextType = SystemContext;

	step() {
		const system = this.context;

		system.step();
	}

	reset() {
		const system = this.context;

		system.reset();
	}

	render() {
		return <div className={style.menubar}>
			<DropDown title="System">
				<MenuItem title={this.context.running ? "stop" : "run"} onClick={() => (this.context.running = !this.context.running)} />
				<Seperator />
				<MenuItem title="step" onClick={() => this.step()} />
				<MenuItem title="reset" onClick={() => this.reset()} />
			</DropDown>
		</div>
	}
}
