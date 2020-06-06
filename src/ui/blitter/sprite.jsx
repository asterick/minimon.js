import React, { Component } from "react";

import SystemContext from "../context";

function reverse(w) {
    w = ((w & 0b1010101010101010) >> 1) | ((w & 0b0101010101010101) << 1);
    w = ((w & 0b1100110011001100) >> 2) | ((w & 0b0011001100110011) << 2);
    w = ((w & 0b1111000011110000) >> 4) | ((w & 0b0000111100001111) << 4);
    return (w >> 8) | (w << 8)
}

export default class Sprite extends Component {
	static contextType = SystemContext;

	constructor(props) {
        super(props);
        this.canvas = React.createRef();
    }

    componentDidMount() {
        this.ctx = this.canvas.current.getContext('2d');
        this.img = this.ctx.getImageData(0, 0, 16, 16);
        this.px = new Uint32Array(this.img.data.buffer);

        this._repaint();
    }

    componentDidUpdate() {
        this._repaint();
    }

    _repaint() {
        const system = this.context.system;

        let tile = this.props.tile;
        let flags = this.props.flags;

        if (tile >= 0x2100 && tile == this.tile && flags == this.flags) return ;
        this.tile = tile;
        this.flags = flags;

        for (let b = 0; b < 16; b += 8) {
            for (let x = 0; x < 8; x++, tile++) {
                let mask = system.read(tile) | (system.read(tile + 8) << 8);
                let color = system.read(tile + 16) | (system.read(tile + 24) << 8);
                let dx = (flags & 0b0001) ? (15 - x - b) : (x + b);

                if (this.props.flags & 0b0010) {
                    color = reverse(color);
                    mask = reverse(mask);
                }

                if (flags & 0b0100) {
                    color = ~color;
                }

                for (let y = 0; y < 16; y++) {
                    if ((mask >> y) & 1) {
                        this.px[y*16+dx] = 0;
                    } else if(((color >> y) & 1)) {
                        this.px[y*16+dx] = 0xFF000000;
                    } else {
                        this.px[y*16+dx] = 0xFFCCCCCC;
                    }
                }
            }
            tile += 24;
        }

        this.ctx.putImageData(this.img, 0, 0);
    }

    render() {
        return <canvas style={this.props.style} ref={this.canvas} width={16} height={16} />
    }
}
