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

class MenuItem extends Component {
	constructor(props) {
		super(props);

		this.keyevent = (e) => {
			const keybind = this.props.keybind;

			if (!keybind) return ;
			
			const fired = Object.keys(keybind).reduce((o, v) => o && e[v] == keybind[v], true);

			if (fired) {
				this.props.onClick();
				e.preventDefault();
			}
		}
	}

	componentDidMount() {
		document.body.addEventListener('keydown', this.keyevent);
	}

	componentWillUnmount() {
		document.body.removeEventListener('keydown', this.keyevent);
	}

	render() {
		const { title } = this.props;
		
		return <span className={style.menuitem} {... this.props}>{title}</span>
	}
}

const DropDown = ({ title, children }) => 
	<div className={style.dropdown}>
		<span className={style.menuitem}>{title}</span>
		<div className={style.dropchildren}>{ children }</div>
	</div>;

const Seperator = () => <div className={style.seperator}></div>

export default class MenuBar extends Component {
	static contextType = SystemContext;

	render() {
		return <div className={style.menubar}>
			<DropDown title="System">
				<MenuItem title="run" keybind={{code: "KeyS", ctrlKey: true}} ischecked={this.context.running.toString()} onClick={() => (this.context.running = !this.context.running)} />
				<Seperator />
				<MenuItem title="step" keybind={{code:"Space"}} onClick={() => this.context.step()} />
				<MenuItem title="reset" keybind={{code: "KeyR", ctrlKey: true}} onClick={() => this.context.reset()} />
			</DropDown>
		</div>
	}
}
