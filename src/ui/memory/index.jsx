import React, { Component } from "react";
import { AutoSizer, List } from 'react-virtualized';

import style from "./style.css";

const TOP_OF_MEMORY = 0x1FFFFF;

function toHex(i, c) {
	let v = i.toString(16).toUpperCase();
	while (v.length < c) v = "0" + v;
	return v;
}

class MemoryCell extends Component {
	render() {
		return (
			<span className={style['byte-cell']}>{toHex(this.props.value, 2)}</span>
		);
	}
}

function rowRenderer ({ key, index, parent }) {
	const { system, memoryBottom, bytesPerRow } = parent.props;

	let address = index * bytesPerRow + memoryBottom;

	const data = [<span className={style['address']} key={-1}>{toHex(address,6)}</span>]

	for (let i = 0; i < bytesPerRow; i++, address++) 
		data.push(<MemoryCell key={address} value={system.read(address)} />);

	return (
		<div key={key} style={style}>
			{ data }
		</div>
	)
}

class MemoryEditor extends Component {
	render() {
		return (
			<AutoSizer className={style.memory}>
				{({ height, width }) => {
					const bytesPerRow = 0x20;//Math.floor((width - 66 - 15) / 22) || 1;

					return (
						<List
							system={this.props.system}
							bytesPerRow={bytesPerRow}
							memoryTop={this.props.memoryTop}
							memoryBottom={this.props.memoryBottom}

							width={width}
							height={height}
							rowCount={Math.ceil((this.props.memoryTop - this.props.memoryBottom) / bytesPerRow)}
							rowHeight={20}
							rowRenderer={rowRenderer} />
					)
				}}
			</AutoSizer>
		);
	}
}

MemoryEditor.defaultProps = {
	memoryBottom: 0,
	memoryTop: TOP_OF_MEMORY
};

export default MemoryEditor;
