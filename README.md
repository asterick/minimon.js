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
| S1C88 Core        | 100%       | Loop complete, instructions unverfied |
| Control Registers | 100%       | Unverified                            |
| IRQ System        | 100%       | Unverified                            |
| RTC Timer         | 100%       | Unverified                            |
| 256Hz Timer       | 100%       | Unverified                            |
| LCD (SED1565)     | 80%        | Most commands implemented             |
| Blitter           | 100%       | Unverified sprites and scrolling      |
| General Timers    | 90%        | Comparison IRQs                       |
| Input Ports       | 50%        | Debounce circuit not done             |
| Output Ports      | 0%         |                                       |
| GPIO Ports        | 0%         |                                       |
| Sound Emulation   | 0%         |                                       |
| ADC Emulation     | 0%         | Only used for low battery detection   |
| EEPROM Emulation  | 100%       | Not wired into GPIO                   |
