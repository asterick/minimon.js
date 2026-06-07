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

/*
 * Hand-written mirror of the reflection tables in core/wasm/main.cc
 * (get_description). The runtime object is built dynamically by
 * struct() in ./state.ts; these interfaces describe what that builder
 * produces for the current Machine::State layout.
 *
 * KEEP IN SYNC: when a field is added to the reflection tables in
 * core/wasm/main.cc, add it here as well.
 */

// C++ `bool` fields are backed by a byte in WASM memory: reads produce
// 0/1, writes accept either form and coerce through DataView.setUint8.
export type WasmBool = number | boolean;

export interface CpuState {
    sc: number;
    cc: number;
    br: number;
    ep: number;
    xp: number;
    yp: number;
    cb: number;
    nb: number;
    a: number;
    b: number;
    l: number;
    h: number;
    ba: number;
    hl: number;
    pc: number;
    sp: number;
    ix: number;
    iy: number;
}

export interface IrqState {
    enable: WasmBool;
    active: WasmBool;
    /** sic — the reflection table in main.cc spells the field "priorty" */
    priorty: Uint8Array;
    next_priority: number;
    next_irq: number;
}

export interface Tim256State {
    running: WasmBool;
    value: number;
}

export interface RtcState {
    running: WasmBool;
    value: number;
    prescale: number;
}

export interface ControlState {
    data: Uint8Array;
}

export interface EepromState {
    data: Uint8Array;
    data_in: number;
    data_out: number;
    clock_in: number;
    address: number;
    mode: number;
    shift: number;
    bit: number;
}

export interface GpioState {
    output: number;
    direction: number;
    eeprom: EepromState;
}

export interface LcdState {
    gddram: Uint8Array[];
    read_buffer: number;
    volume: number;
    column_address: number;
    page_address: number;
    start_address: number;

    rmw_mode: WasmBool;
    adc_select: WasmBool;
    setting_volume: WasmBool;
    display_enable: WasmBool;
    reverse_display: WasmBool;
    all_on: WasmBool;
    reverse_com_scan: WasmBool;
    static_indicator: WasmBool;
    lcd_bias: WasmBool;

    resistor_ratio: number;
    operating_mode: number;
    scanline: number;

    overflow: number;
}

export interface InputState {
    bytes: Uint8Array;
    interrupt_direction: number;
    input_state: number;
    dejitter: number;
}

export interface BlitterState {
    enables: number;
    rate_control: number;
    map_base: number;
    sprite_base: number;
    scroll_x: number;
    scroll_y: number;
    divider: number;
}

export interface BlitterSprite {
    x: number;
    y: number;
    tile: number;
    flags: number;
}

export interface BlitterOverlay {
    framebuffer: Uint8Array[];
    oam: BlitterSprite[];
    /** flat alias over the same memory as framebuffer */
    map: Uint8Array;
}

export interface TimerInstance {
    /** raw flag bytes: bits 0-3 input/preset/running/output; bit 7 of lo_flags is mode16 */
    lo_flags: number;
    hi_flags: number;
    preset: number;
    compare: number;
    count: number;

    lo_clock_ratio: number;
    lo_clock_ctrl: WasmBool;
    lo_clock_source: WasmBool;
    hi_clock_ratio: number;
    hi_clock_ctrl: WasmBool;
    hi_clock_source: WasmBool;
}

export interface TimersState {
    timer: TimerInstance[];
    osc1_enable: WasmBool;
    osc3_enable: WasmBool;
    osc1_prescale: number;
    osc3_prescale: number;
}

export interface MachineState {
    cpu: CpuState;
    ctrl: ControlState;
    gpio: GpioState;
    rtc: RtcState;
    irq: IrqState;
    tim256: Tim256State;
    ram: Uint8Array;
    cartridge: Uint8Array;
    lcd: LcdState;
    input: InputState;
    blitter: BlitterState;
    overlay: BlitterOverlay;
    timers: TimersState;
    bus_cap: number;
    clocks: number;
    osc1_overflow: number;
    status: number;
}
