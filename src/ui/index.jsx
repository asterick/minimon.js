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
import 'react-virtualized/styles.css';

import Memory from "./memory";
import Disassembler from "./disassemble";
import Registers from "./registers";
import Screen from "./screen";
import MenuBar from "./menubar";
import SystemContext from "./context";

import style from "./style.scss";

export default class MinimonDebugger extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super(props);

		this.state = {
			fullscreen: false
		};
	}

    onDragOver (e) {
        e.preventDefault();
        e.dataTransfer.dropEffect = "copy";
        this.setState({ dragging: true });
    }

    onDragLeave () {
        this.setState({ dragging: false });
    }

	componentDidMount() {
		this.context.update = () => this.forceUpdate();
	}

	componentWillUnmount() {
		delete this.context.update;
	}

    onDrop (e) {
        e.preventDefault();

        var file = e.dataTransfer.files[0],
            reader = new FileReader();

        reader.onload = (e) => {
        	this.context.load(e.target.result);
        };

        this.setState({ dragging: false }, function () {
            reader.readAsArrayBuffer(file);
        });
    }

    innerUI() {
    	if (this.state.fullscreen) {
    		return <div className={style.debugger}>
    			<div className={`${style.screen} ${style.fullsize}`}>
					<Screen />
				</div>
			</div>
    	} else {
			const system = this.context;

    		return <div className={style.debugger}>
    			<div className={style.sidebar}>
					<div className={style.screen}>
						<Screen />
					</div>
					<Registers />
					<Memory baseAddress={0x1000} memory={system.state.ram} />
				</div>
				<div className={style.document}>
					<div className={style.viewer}>
						<Disassembler target={system.state.cpu.pc} />
					</div>
					<div className={style.viewer}>

					</div>
				</div>
			</div>;
    	}
    }

	render() {
		return <div 
            onDragOver={(e) => this.onDragOver(e)}
            onDragLeave={(e) => this.onDragLeave(e)}
            onDrop={(e) => this.onDrop(e)}
			className={style.mainview}>

			<MenuBar />
			
			{this.innerUI()}
		</div>
	}
}
