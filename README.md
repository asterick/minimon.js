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
| IRQ System        | 100%       | Unverified                            |
| RTC Timer         | 100%       | Unverified                            |
| 256Hz Timer       | 100%       | Unverified                            |
| LCD (SED1565)     | 80%        | Most commands implemented             |
| Blitter           | 60%        | No sprites / IRQs                     |
| General Timers    | 0%         |                                       |
| Control Registers | 0%         |                                       |
| I/O / Input Ports | 0%         |                                       |
| EEPROM Emulation  | 0%         |                                       |
| Sound Emulation   | 0%         |                                       |
| ADC Emulation     | 0%         | Only used for low battery detection   |
