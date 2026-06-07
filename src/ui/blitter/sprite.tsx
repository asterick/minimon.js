import { useEffect, useRef } from "react";

import { useSystem } from "../context";

function reverse(w: number): number {
    w = ((w & 0b1010101010101010) >> 1) | ((w & 0b0101010101010101) << 1);
    w = ((w & 0b1100110011001100) >> 2) | ((w & 0b0011001100110011) << 2);
    w = ((w & 0b1111000011110000) >> 4) | ((w & 0b0000111100001111) << 4);
    return (w >> 8) | (w << 8)
}

interface SpriteProps {
    tile: number;
    flags: number;
    style?: React.CSSProperties;
}

export default function Sprite({ tile, flags, style }: SpriteProps) {
    const system = useSystem();
    const canvas = useRef<HTMLCanvasElement>(null);
    const painted = useRef<{ tile: number, flags: number } | null>(null);

    // Repaint after every render; the parent re-renders on machine state
    // changes, so tile data stays current
    useEffect(() => {
        const ctx = canvas.current?.getContext('2d');
        if (!ctx) return;

        // Tiles in cartridge ROM can't change; skip redundant repaints
        if (tile >= 0x2100 && painted.current?.tile == tile && painted.current?.flags == flags) return;
        painted.current = { tile, flags };

        const img = ctx.getImageData(0, 0, 16, 16);
        const px = new Uint32Array(img.data.buffer);

        let address = tile;

        for (let b = 0; b < 16; b += 8) {
            for (let x = 0; x < 8; x++, address++) {
                let mask = system.read(address) | (system.read(address + 8) << 8);
                let color = system.read(address + 16) | (system.read(address + 24) << 8);
                let dx = (flags & 0b0001) ? (15 - x - b) : (x + b);

                if (flags & 0b0010) {
                    color = reverse(color);
                    mask = reverse(mask);
                }

                if (flags & 0b0100) {
                    color = ~color;
                }

                for (let y = 0; y < 16; y++) {
                    if ((mask >> y) & 1) {
                        px[y*16+dx] = 0;
                    } else if(((color >> y) & 1)) {
                        px[y*16+dx] = 0xFF000000;
                    } else {
                        px[y*16+dx] = 0xFFCCCCCC;
                    }
                }
            }
            address += 24;
        }

        ctx.putImageData(img, 0, 0);
    });

    return <canvas style={style} ref={canvas} width={16} height={16} />
}
