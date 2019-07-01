import React, { Component } from "react";
import { AutoSizer, List } from 'react-virtualized';

import classes from "./style.css";

const TOP_OF_MEMORY = 0x1FFFFF;

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class MemoryCell extends Component {
	render() {
		return (
			<span className={classes['byte-cell']}>{toHex(this.props.value, 2)}</span>
		);
	}
}

function rowRenderer ({ key, style, index, parent }) {
	const { system, memoryBottom, memoryTop, bytesPerRow } = parent.props;
	const addrLength = memoryTop.toString(16).length;
	const data = [];

	let address = index * bytesPerRow + memoryBottom;

	for (let i = 0, a = address; i < bytesPerRow && a <= memoryTop; i++, a++) 
		data.push(<MemoryCell key={a} value={system.read(a)} />);

	return (
		<div key={key} style={style}>
			<span className={classes['address']}>{toHex(address, addrLength)}</span>
			{ data }
		</div>
	)
}

class Memory extends Component {
	render() {
		return (
			<AutoSizer className={classes['memory']}>
				{({ height, width }) => {
					console.log(height)
					return (
						<List
							system={this.props.system}
							bytesPerRow={this.props.bytesPerRow}
							memoryTop={this.props.memoryTop}
							memoryBottom={this.props.memoryBottom}

							width={width}
							height={height}
							rowCount={Math.ceil((this.props.memoryTop - this.props.memoryBottom) / this.props.bytesPerRow)}
							rowHeight={20}
							rowRenderer={rowRenderer}
							/>
					)
				}}
			</AutoSizer>
		);
	}
}

Memory.defaultProps = {
	bytesPerRow: 0x8,
	memoryBottom: 0,
	memoryTop: TOP_OF_MEMORY
};

export default Memory;
