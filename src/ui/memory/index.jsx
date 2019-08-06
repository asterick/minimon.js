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

function rowRenderer ({ key, style, index, parent }) {
	const { memory, baseAddress, bytesPerRow } = parent.props;
	const addrLength = (baseAddress + memory.length - 1).toString(16).length;
	const data = [];

	let address = index * bytesPerRow;

	for (let i = 0, a = address; i < bytesPerRow && a < memory.length; i++, a++) 
		data.push(<span key={a} className={classes['byte-cell']}>{toHex(memory[a], 2)}</span>);

	return (
		<div className={classes.dataRow} key={key} style={style}>
			<span className={classes['address']}>{toHex(address + baseAddress, addrLength)}</span>
			{ data }
		</div>
	)
}

class Memory extends Component {
	constructor(props) {
		super(props);

		this._list = React.createRef();
	}

	componentDidUpdate() {
		this._list.current.forceUpdateGrid();
	}

	render() {
		return <div className={classes['memory']}>
			<AutoSizer>
				{({ height, width }) => {
					return (
						<List
							bytesPerRow={this.props.bytesPerRow}
							baseAddress={this.props.baseAddress}
							memory={this.props.memory}

							ref={this._list}

							width={width}
							height={height}
							rowCount={Math.ceil(this.props.memory.length / this.props.bytesPerRow)}
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
	bytesPerRow: 0x8
};

export default Memory;
