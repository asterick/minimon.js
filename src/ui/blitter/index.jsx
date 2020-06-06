
import React, { Component } from "react";
import Sprite from "./sprite";
import Map from "./map";

import SystemContext from "../context";

const map_sizes = {
    [0b000000]: { w: 12, h: 16 },
    [0b010000]: { w: 16, h: 12 },
    [0b100000]: { w: 24, h:  8 },
    [0b110000]: { w: 24, h: 16 },
}

function toHex(v, c) {
	v = v.toString(16).toUpperCase();
    while (v.length < c) 
    v = "0" + v;
	return v;
}

export default class Blitter extends Component {
	static contextType = SystemContext;

    render() {
        const system = this.context.system;
        const oam = system.state.overlay.oam;

        const { scroll_x, scroll_y, sprite_base, enables } = system.state.blitter;
        const { map } = system.state.overlay;
        let rows = [];
        
        const {w, h} = map_sizes[enables & 0b110000];

        const baseX = 96;
        const baseY = 64;

        if (scroll_x >= w*8) scroll_x = 0;
        if (scroll_y >= h*8) scroll_y = 0;

        let data = [];
        let sprites = [];

        for (let i = 0; i < 24; i++) {
            let { x, y, tile, flags } = oam[i];
            tile = tile * 64 + sprite_base;
            
            x = (x & 0x7F) + baseX - 16;
            y = (y & 0x7F) + baseY - 16;

            data.push(<div key={i}>
                <Sprite style={{ background:'white', border: '1px solid black' }} tile={tile} flags={flags} />
                {toHex(x & 0x7F, 2)}
                {toHex(y & 0x7F, 2)}
                {(flags & 0b1000) ? "enable" : null}
                {(flags & 0b0100) ? "invert" : null}
                {(flags & 0b0010) ? "y-flip" : null}
                {(flags & 0b0001) ? "x-flip" : null}
            </div>);

            if (flags & 0b1000) {
                sprites.push(<Sprite
                    key={i}
                    style={{ 
                        position: 'absolute', 
                        zIndex: 24-i, 
                        left: `${x}px`,
                        top: `${y}px`
                    }} tile={tile} 
                    flags={flags} 
                    />);
            }
        }

        return <div>
            <div style={{ 
                position: 'relative', 
                width: `${96*3 + 16}px`,
                height: `${64*3 + 16}px`,
                background: '#aaa'
                }}>
                <Map style={{ 
                    position: 'absolute', 
                    left: baseX - scroll_x,
                    top: baseY - scroll_y
                }}/>
                { sprites }
                <div style={{
                    position: 'absolute', 
                    border: '1px solid #C0C',
                    left: baseX - 1,
                    top: baseY - 1,
                    width: 96,
                    height: 64
                }}
                />
            </div>
            { data }
        </div>;
    }
}
