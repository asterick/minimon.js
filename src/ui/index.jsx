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

		this.state = {};
	}

    onDragOver (e) {
        e.preventDefault();
        e.dataTransfer.dropEffect = "copy";
        this.setState({ dragging: true });
    }

    onDragLeave () {
        this.setState({ dragging: false });
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

	render() {
		const system = this.context;

		return <div 
            onDragOver={(e) => this.onDragOver(e)}
            onDragLeave={(e) => this.onDragLeave(e)}
            onDrop={(e) => this.onDrop(e)}
			className={style.mainview}>

			<MenuBar repaint={() => this.setState({})} />
			<div className={style.debugger}>
				<div className={style.sidebar}>
					<Screen />
					<Registers />
					<Memory memoryBottom={0x1000} memoryTop={0x20FF} />
				</div>
				<div className={style.document}>
					<Disassembler target={system.translate(system.registers.pc)} />
				</div>
			</div>
		</div>
	}
}
