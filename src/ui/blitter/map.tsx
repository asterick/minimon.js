import { useEffect, useRef } from "react";

import { useSystem, useSystemState } from "../context";

const map_sizes: Record<number, { w: number, h: number }> = {
    [0b000000]: { w: 12, h: 16 },
    [0b010000]: { w: 16, h: 12 },
    [0b100000]: { w: 24, h:  8 },
    [0b110000]: { w: 24, h: 16 },
}

interface MapTileProps {
    tile: number;
    style?: React.CSSProperties;
}

function MapTile({ tile, style }: MapTileProps) {
    const system = useSystem();
    const canvas = useRef<HTMLCanvasElement>(null);
    const painted = useRef<number | null>(null);

    useEffect(() => {
        const ctx = canvas.current?.getContext('2d');
        if (!ctx) return;

        const { enables, map_base } = system.state.blitter;

        let address = tile * 8 + map_base;

        // Tiles in cartridge ROM can't change; skip redundant repaints
        if (address >= 0x2100 && address == painted.current) return;
        painted.current = address;

        const invert_map = enables & 0b00001;

        const img = ctx.getImageData(0, 0, 8, 8);
        const px = new Uint32Array(img.data.buffer);

        for (let x = 0; x < 8; x++) {
            let byte = system.read(address++);

            for (let y = 0; y < 8; y++) {
                px[y*8+x] = (!((byte >> y) & 1) != !invert_map) ? 0xFF000000 : 0xFFCCCCCC;
            }
        }

        ctx.putImageData(img, 0, 0);
    });

    return <canvas style={style} ref={canvas} width={8} height={8} />
}

interface MapViewProps {
    style?: React.CSSProperties;
}

export default function MapView({ style }: MapViewProps) {
    const system = useSystem();
    useSystemState();

    const { enables } = system.state.blitter;
    const { map } = system.state.overlay;
    const tiles = [];

    const {w, h} = map_sizes[enables & 0b110000];
    let ta = 0;

    for (let y = 0; y < h; y++) {
        for (let x = 0; x < w; x++) {
            tiles.push(<MapTile key={ta} style={{ position: 'absolute', top: `${y*8}px`, left: `${x*8}px`}} tile={map[ta++]} />);
        }
    }

    return <div style={style}>{tiles}</div>;
}
