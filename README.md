minimon.js
==========
Minimon for the web

Notes
-----
* Emulation core is little-endian only
* Libretro does not preserve eeprom or RTC between sessions

Emulation Status
----------------

| System            | Completion | Notes                                 |
| ----------------- | ---------- | ------------------------------------- |
| S1C88 Core        | 100%       |                                       |
| IRQ System        | 100%       |                                       |
| RTC Timer         | 100%       |                                       |
| 256Hz Timer       | 100%       |                                       |
| Blitter           | 100%       |                                       |
| General Timers    | 100%       |                                       |
| Sound Emulation   |  80%       | Works in libretro, not wired to web   |
| LCD (SED1565)     |  80%       | Most commands implemented             |
| Control Registers |  80%       | Bus disable not emulated              |
| Input Ports       |  50%       | Debounce circuit not done             |
| Output Ports      |   0%       |                                       |
| GPIO Ports        |  25%       |                                       |
| ADC Emulation     |  20%       | Stubbed with constant value           |
| EEPROM Emulation  | 100%       |                                       |
