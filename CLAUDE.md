# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

minimon.js is a Pokemon Mini emulator: a C++ emulation core compiled to WebAssembly, driven by a React-based debugger UI in the browser. The same core also builds as a libretro (RetroArch) core.

## Workflow

`main` is protected: no direct commits — all changes go on a branch named `<committer-github-username>/<change-to-be-made>` (e.g. `asterick/register-portability`) and land via PR (no review approval required). If a change grows beyond what the branch name describes but belongs in the same commit, rename the branch to match the combined scope and retarget any open PRs. Push the branch to origin whenever the work reaches a clean, mergeable state.

## Commands

- `npm start` — regenerate the instruction table, then run the Vite dev server on port 9000. Note: this does **not** rebuild the WASM core; run `npm run core` first if core C++ changed.
- `npm run core` — build the WASM core (`core/ && make wasm`) → outputs `public/libminimon.wasm` and `public/libminimon.tracing.wasm` (same code compiled with `-D TRACING`).
- `npm run table` — regenerate `src/system/table.ts` via `tools/convert.py`.
- `npm run check` — regenerate the table, then `tsc --noEmit`. Run this after changing any `.ts` file.
- `npm run build` — table + core + production Vite build into `dist/` (contents of `public/`, including the wasm binaries, are copied in).
- `npm run preview` — serve the production `dist/` build.
- `npm run retroarch` — build and test the libretro core (`core/libretro/`).

Type checking is `npm run check` (not part of the build — Vite/esbuild only transpiles). The entire `src/` tree is strict TypeScript; run `npm run check` after changing any `.ts`/`.tsx` file. There is no linter.

The core has a regression harness in `core/test/`: `make -C core/test check` builds the core natively and diffs per-virtual-second hashes of semantic machine state (registers + RAM + LCD gddram — never raw struct bytes) against checked-in baselines, for a BIOS-only boot and a synthetic peripheral workout (blitter map+sprites, timers in both widths, audio, input edge IRQs). Run it after any core change; CI runs it on every PR. An intentional behavior change means regenerating with `make -C core/test baseline` and calling out the diff in the PR. The synthetic runs don't cover cartridge-driven code paths — for layout or behavior-sensitive changes, also verify a real ROM locally: `core/test/harness rom.min 30` diffed before vs after.

Building the WASM core uses the system WASI toolchain: `clang` (wasm32 backend), `wasm-ld`, and the wasi-libc sysroot — `apt install lld wasi-libc` on Debian/Ubuntu. A non-standard sysroot location can be passed via `WASI_SYSROOT`/`WASI_LIBDIR` (see `core/wasm/Makefile`).

## Generated files (do not edit by hand)

`tools/s1c88.csv` is the single source of truth for the S1C88 instruction set. Two scripts generate code from it:

- `tools/table.py` → `core/table.h` (consumed by the C++ core; rebuilt by `core/Makefile`)
- `tools/convert.py` → `src/system/table.ts` (consumed by the JS disassembler; rebuilt by `npm run table`)

Both outputs are gitignored. `public/` (Vite's static dir, where the wasm binaries are built) is gitignored except `favicon.png`, and `dist/` (Vite's build output) is gitignored entirely.

## Architecture

Three layers, with the C++ core shared between two front-ends:

1. **Emulation core** — `core/src/*.cc` + `core/include/*.h`. S1C88 CPU (`operations.cc` uses the generated `table.h`), plus hardware peripherals each in their own file: irq, rtc, tim256, timers, blitter, lcd (SED1565), audio, eeprom, gpio, input, control. `machine.cc/.h` ties them together as `Machine::State`. The core is little-endian only.

2. **WASM bridge** — `core/wasm/main.cc` exports the C ABI (`get_machine`, `cpu_advance`, `cpu_step`, `cpu_read/write`, etc.) and, critically, `get_description`: a reflection table (`StructDecl`/`FieldDecl`) describing the entire `Machine::State` memory layout. On the JS side, `src/system/state.ts` parses that reflection blob and builds a live JS object view (DataView/typed arrays directly over WASM memory) — this is how the debugger UI reads/writes registers and hardware state without per-field glue code. The shape of that object is described by the hand-written interfaces in `src/system/machine-state.ts`. When adding fields to machine state that the UI should see, they must be added to the reflection tables in `core/wasm/main.cc` **and** mirrored in `machine-state.ts`.

3. **Web UI** — `src/system/index.ts` (`Minimon` class) fetches the wasm binary (swapping between normal/tracing builds when `.tracing` is toggled), wires keyboard input, WebAudio (`src/system/audio.ts`), breakpoints, and EEPROM/RTC persistence. `src/ui/` is the React debugger: a dockview window layout with native controls, with panels for the screen (WebGL, shaders in `src/ui/screen/shaders/*.glsl`), disassembly, registers, memory (virtualized via `@tanstack/react-virtual`), and blitter/sprite/map viewers. Panels are function components; the system instance comes from React context via `useSystem()` (`src/ui/context.tsx`), and panels subscribe to machine-state changes with `useSystemState()` (`useSyncExternalStore` over a version counter on `Minimon` — machine state is a live mutable view over WASM memory, so the version number serves as the snapshot and `update()` notifies, coalesced to animation frames).

The libretro front-end (`core/libretro/libretro.cc`) wraps the same core; it does not preserve EEPROM or RTC between sessions.

## Build specifics (Vite)

- `index.html` at the repo root is the Vite entry; it loads `/src/index.tsx` as a module.
- `*.module.less` files are CSS modules (Vite convention); plain `.less`/`.css` (including third-party styles imported from JS) load globally.
- GLSL shaders import with the `?raw` suffix (`import shader from './x.glsl?raw'`).
- JSX only parses in `.jsx`/`.tsx` files; there is no type checking in the build.

## Roadmap

Agreed ongoing work, one branch/PR per item (see Workflow). Update this list as items land.

**Core portability** — COMPLETE: the core is compiler- and platform-generic — no endianness assumptions, no packed bitfields or type-punning unions, just plain fields and explicit bit math. CPU register state (PR #39), peripheral register files incl. the Timer reflection fixes (PR #48), guest-memory overlays (PR #49), layout `static_assert`s + libretro `link.T` (PR #50). When changing core state layout, verify bit-identical behavior with the regression harness in `core/test/` (see Commands). Note that layout changes invalidate raw-`memcpy` libretro savestates; there is no savestate versioning.

**UI modernization** — COMPLETE: Vite (PR #35), strict-TS system layer (PR #37), hooks/`useSyncExternalStore` refactor (PR #41), library swaps (`@tanstack/react-virtual` PR #42, native controls PR #43, dockview PR #44), AudioWorklet (PR #45), GitHub Actions CI (PR #47). CI runs a type check plus the full build (instruction table, WASM core, Vite) on every PR and push to main — see `.github/workflows/ci.yml`.
