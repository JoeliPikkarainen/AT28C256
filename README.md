
# AT28C256
This is project to program AT28C256 EEPROM with ATmega 2560

TODO: Refactor the com protocol, make it simple.

## Connections Arduino to AT28C256

| Arduino       | AT28C256      | TYPE  |
| ------------- |:-------------:| -----:|
| 22| A0 | ADDRESS |
| 24| A1 | ADDRESS |
| 26| A2 | ADDRESS |
| 28| A3 | ADDRESS |
| 30| A4 | ADDRESS |
| 32| A5 | ADDRESS |
| 34| A6 | ADDRESS |
| 36| A7 | ADDRESS |
| 38| A8 | ADDRESS |
| 40| A9 | ADDRESS |
| 42| A10 | ADDRESS |
| 44| A11| ADDRESS |
| 46| A12| ADDRESS |
| 48| A13| ADDRESS |
| 50| A14| ADDRESS |

### I/O
| Arduino       | AT28C256      | TYPE  |
| ------------- |:-------------:| -----:|
| 31| I/O0 | DATA |
| 33| I/O1 | DATA |
| 35| I/O2 | DATA |
| 37| I/O3 | DATA |
| 39| I/O4 | DATA |
| 41| I/O5 | DATA |
| 43| I/O6 | DATA |
| 45| I/O7 | DATA |

### Signals

| Arduino       | AT28C256      | TYPE  |
| ------------- |:-------------:| -----:|
| 5| WED | SIGNAL |
| 2| OED | SIGNAL |
| 4| CED | SIGNAL |


Power of the EEPROM is from Arduino  PWR 5V and GND

