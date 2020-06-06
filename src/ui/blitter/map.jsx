import React, { Component } from "react";

import SystemContext from "../context";

const map_sizes = {
    [0b000000]: { w: 12, h: 16 },
    [0b010000]: { w: 16, h: 12 },
    [0b100000]: { w: 24, h:  8 },
    [0b110000]: { w: 24, h: 16 },
}

class MapTile extends Component {
	static contextType = SystemContext;

	constructor(props) {
        super(props);
        this.canvas = React.createRef();
    }

    componentDidMount() {
        this.ctx = this.canvas.current.getContext('2d');
        this.img = this.ctx.getImageData(0, 0, 8, 8);
        this.px = new Uint32Array(this.img.data.buffer);

        this._repaint();
    }

    componentDidUpdate() {
        this._repaint();
    }

    _repaint() {
        const system = this.context.system;
        const { enables, map_base } = system.state.blitter;

        let tile = this.props.tile * 8 + map_base;

        if (tile >= 0x2100 && tile == this.tile) return ;
        this.tile = tile;

        const invert_map = enables & 0b00001;
        const {w, h} = map_sizes[enables & 0b110000];

        const wt = w * 8;
        const ht = h * 8;

        let ta = 0;

        for (let x = 0; x < 8; x++) {
            let byte = system.read(tile++);

            for (let y = 0; y < 8; y++) {
                this.px[y*8+x] = (!((byte >> y) & 1) != !invert_map) ? 0xFF000000 : 0xFFCCCCCC;
            }
        }

        this.ctx.putImageData(this.img, 0, 0);
    }

    render() {
        return <canvas style={this.props.style} ref={this.canvas} width={8} height={8} />
    }
}

export default class MapView extends Component {
	static contextType = SystemContext;

    render() {
        const system = this.context.system;

        const { scroll_x, scroll_y, enables } = system.state.blitter;
        const { map } = system.state.overlay;
        let tiles = [];
        
        const {w, h} = map_sizes[enables & 0b110000];
        let ta = 0;

        for (let y = 0; y < h; y++) {
            for (let x = 0; x < w; x++) {
                tiles.push(<MapTile key={ta} style={{ position: 'absolute', top: `${y*8}px`, left: `${x*8}px`}} tile={map[ta++]} />);
            }
        }

        return <div style={this.props.style}>{tiles}</div>;
    }
}
