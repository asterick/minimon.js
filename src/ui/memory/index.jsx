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

import classes from "./style.scss";
import SystemContext from "../context";

const TOP_OF_MEMORY = 0x1FFFFF;

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

const MemoryCell = (props) =>
	<span className={classes['byte-cell']}>{toHex(props.value, 2)}</span>;

function rowRenderer ({ key, style, index, parent }) {
	const { system, memoryBottom, memoryTop, bytesPerRow } = parent.props;
	const addrLength = memoryTop.toString(16).length;
	const data = [];

	let address = index * bytesPerRow + memoryBottom;

	for (let i = 0, a = address; i < bytesPerRow && a <= memoryTop; i++, a++) 
		data.push(<MemoryCell key={a} value={system.read(a)} />);

	return (
		<div className={classes.dataRow} key={key} style={style}>
			<span className={classes['address']}>{toHex(address, addrLength)}</span>
			{ data }
		</div>
	)
}

class Memory extends Component {
	static contextType = SystemContext;

	constructor(props) {
		super(props);

		this._list = React.createRef();
	}

	componentDidUpdate() {
		this._list.current.forceUpdateGrid();
	}

	render() {
		const system = this.context;

		return <div className={classes['memory']}>
			<AutoSizer>
				{({ height, width }) => {
					return (
						<List
							system={system}
							bytesPerRow={this.props.bytesPerRow}
							memoryTop={this.props.memoryTop}
							memoryBottom={this.props.memoryBottom}
							ref={this._list}

							width={width}
							height={height}
							rowCount={Math.ceil((this.props.memoryTop - this.props.memoryBottom) / this.props.bytesPerRow)}
							rowHeight={20}
							rowRenderer={rowRenderer}
							/>
					)
				}}
			</AutoSizer>
		</div>
	}
}

Memory.defaultProps = {
	bytesPerRow: 0x8,
	memoryBottom: 0,
	memoryTop: TOP_OF_MEMORY
};

export default Memory;
