# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

minimon.js is a Pokemon Mini emulator: a C++ emulation core compiled to WebAssembly, driven by a React-based debugger UI in the browser. The same core also builds as a libretro (RetroArch) core.

## Workflow

`main` is protected: no direct commits — all changes go on a branch named `<committer-github-username>/<change-to-be-made>` (e.g. `asterick/register-portability`) and land via PR (no review approval required). If a change grows beyond what the branch name describes but belongs in the same commit, rename the branch to match the combined scope and retarget any open PRs. Push the branch to origin whenever the work reaches a clean, mergeable state.

## Commands

- `npm start` — regenerate the instruction table, then run webpack-dev-server on port 9000. Note: this does **not** rebuild the WASM core; run `npm run core` first if core C++ changed.
- `npm run core` — build the WASM core (`core/ && make wasm`) → outputs `public/libminimon.wasm` and `public/libminimon.tracing.wasm` (same code compiled with `-D TRACING`).
- `npm run table` — regenerate `src/system/table.ts` via `tools/convert.py`.
- `npm run build` — table + core + production webpack bundle into `public/`.
- `npm run retroarch` — build and test the libretro core (`core/libretro/`).

There are no tests and no linter. There is no tsc typecheck step — TypeScript/JSX is transpiled by Babel only (the repo is mid-migration from JS to TS).

Building the WASM core requires `clang` with the wasm32 target, `wasm-ld` (falls back to `wasm-ld-10`), python3, and the `core/wasm/wasi-libc` git submodule (checked out and prebuilt; its Makefile invocation is commented out in `core/wasm/Makefile`).

## Generated files (do not edit by hand)

`tools/s1c88.csv` is the single source of truth for the S1C88 instruction set. Two scripts generate code from it:

- `tools/table.py` → `core/table.h` (consumed by the C++ core; rebuilt by `core/Makefile`)
- `tools/convert.py` → `src/system/table.ts` (consumed by the JS disassembler; rebuilt by `npm run table`)

Both outputs are gitignored. `public/` is also gitignored except `index.html` and `favicon.png` — everything else there is build output.

## Architecture

Three layers, with the C++ core shared between two front-ends:

1. **Emulation core** — `core/src/*.cc` + `core/include/*.h`. S1C88 CPU (`operations.cc` uses the generated `table.h`), plus hardware peripherals each in their own file: irq, rtc, tim256, timers, blitter, lcd (SED1565), audio, eeprom, gpio, input, control. `machine.cc/.h` ties them together as `Machine::State`. The core is little-endian only.

2. **WASM bridge** — `core/wasm/main.cc` exports the C ABI (`get_machine`, `cpu_advance`, `cpu_step`, `cpu_read/write`, etc.) and, critically, `get_description`: a reflection table (`StructDecl`/`FieldDecl`) describing the entire `Machine::State` memory layout. On the JS side, `src/system/state.js` parses that reflection blob and builds a live JS object view (DataView/typed arrays directly over WASM memory) — this is how the debugger UI reads/writes registers and hardware state without per-field glue code. When adding fields to machine state that the UI should see, they must be added to the reflection tables in `core/wasm/main.cc`.

3. **Web UI** — `src/system/index.js` (`Minimon` class) fetches the wasm binary (swapping between normal/tracing builds when `.tracing` is toggled), wires keyboard input, WebAudio (`src/system/audio.js`), breakpoints, and EEPROM/RTC persistence. `src/ui/` is the React debugger: rc-dock window layout + Fluent UI widgets, with panels for the screen (WebGL, shaders in `src/ui/screen/shaders/*.glsl`), disassembly, registers, memory, and blitter/sprite/map viewers. The system instance is passed through React context (`src/ui/context.jsx`).

The libretro front-end (`core/libretro/libretro.cc`) wraps the same core; it does not preserve EEPROM or RTC between sessions.

## Webpack specifics

- `.less` files load as CSS modules; `.glsl` via raw-loader.
- Babel handles `.js/.jsx/.ts/.tsx` (`@babel/preset-typescript` + `@babel/preset-react`), but `resolve.extensions` only lists `.js`/`.jsx` — imports of TS files spell out the extension (e.g. `import * as Table from "./table.ts"`).
