minimon.js
==========
Minimon for the web

Notes
-----
* Emulation core is little-endian only

Emulation Status
----------------

| System            | Completion | Notes                                 |
| ----------------- | ---------- | ------------------------------------- |
| S1C88 Core        | 100%       |                                       |
| IRQ System        | 100%       |                                       |
| RTC Timer         | 100%       |                                       |
| 256Hz Timer       | 100%       |                                       |
| Blitter           | 100%       |                                       |
| LCD (SED1565)     |  80%       | Most commands implemented             |
| Control Registers |  80%       | Bus disable not emulated              |
| General Timers    |  90%       | Audio accumulator                     |
| Input Ports       |  50%       | Debounce circuit not done             |
| Output Ports      |   0%       |                                       |
| GPIO Ports        |  25%       |                                       |
| Sound Emulation   |  80%       | Works in libretro, not wired to web   |
| ADC Emulation     |  20%       | Stubbed with constant value           |
| EEPROM Emulation  | 100%       | Complete                              |
