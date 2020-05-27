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
import { AutoSizer, List } from 'react-virtualized';

import classes from "./style.less";
import SystemContext from "../context";

const TOP_OF_MEMORY = 0x1FFFFF;

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

function rowRenderer ({ key, style, index, parent }) {
	const { memory, baseAddress, bytesPerRow } = parent.props;
	const data = [];

	let address = index * bytesPerRow;

	for (let i = 0, a = address; i < bytesPerRow && a < memory.length; i++, a++) 
		data.push(<span key={a} className={classes.byteCell}>{toHex(memory[a], 2)}</span>);

	return (
		<div className={classes.dataRow} key={key} style={style}>
			<span className={classes.address}>{toHex(address + baseAddress, 6)}</span>
			{ data }
		</div>
	)
}

export default class Memory extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super(props);

		this.state = {};
		this._list = React.createRef();
	}

	componentDidUpdate() {
		this._list.current.forceUpdateGrid();
	}

	measure(r) {
		if (!r) return ;

		let { offsetWidth, offsetHeight } = r.querySelector(".one-element");
		let elementWidth = r.querySelector(".two-element").offsetWidth - offsetWidth;
		let rowHeight = offsetHeight;
		let baseWidth = offsetWidth - elementWidth;

		if (elementWidth != this.state.elementWidth || 
			rowHeight != this.state.rowHeight || 
			baseWidth != this.state.baseWidth
			) {
			this.setState({
				baseWidth,
				elementWidth,
				rowHeight 
			});
		}
	}

	render() {
		const system = this.context.system;
		let memory;

		switch (this.props.memory) {
			case 'ram':
				memory = system.state.ram;
				break ;
			case 'eeprom':
				memory = system.state.gpio.eeprom.data;
				break ;
		}

		return <AutoSizer>
			{({width, height}) => {
				/* Measure row / child sizes */
				if (!this.state.baseWidth) {
					return <div style={{position: 'absolute', visibility: "hidden"}} ref={(r) => this.measure(r)}>
						<div className="one-element" style={{display: 'inline-block'}}>
							<span className={classes.address}>{toHex(0, 6)}</span>
							<span className={classes.byteCell}>00</span>
						</div>
						<div className="two-element" style={{display: 'inline-block'}}>
							<span className={classes.address}>{toHex(0, 6)}</span>
							<span className={classes.byteCell}>00</span>
							<span className={classes.byteCell}>00</span>
						</div>
					</div>;
				} 

				let { baseWidth, elementWidth, rowHeight } = this.state;

				let bytesPerRow = Math.floor((width - baseWidth) / elementWidth) - 2;
				if (bytesPerRow <= 0) return null;

				return (
					<List
						className="Memory"
						baseAddress={this.props.baseAddress}
						memory={memory}

						ref={this._list}

						width={width}
						height={height}
						bytesPerRow={bytesPerRow}
						rowCount={Math.ceil(memory.length / bytesPerRow)}
						rowHeight={rowHeight}
						rowRenderer={rowRenderer}
						/>
				);
			}}
		</AutoSizer>
	}
}
