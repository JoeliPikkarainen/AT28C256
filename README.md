# AT28C256
This is project to program AT28C256 EEPROM with ATmega 2560

DONE: Working write and read functions
DONE: Working test function to fill EEPROM with single value and verify

TODO: Add possibility send .hex file from Serial Monitor to Arduino and program the eeprom
TODO: Make COM_IF for Arduino, needed functions: set_start_address(uint16_t address), verify(char* hexfile, uint16_t start_address)

## Connections Arduino to AT28C256

### Address

20    -   A0

22    -   A1

24    -   A2

26    -   A3

28    -   A4

30    -   A5

32    -   A6

34    -   A7

36    -   A8

38    -   A9

40    -   A10

42    -   A11

43    -   A12

44    -   A13

44    -   A14

46    -   A15

48    -   NC

### I/O

31    -   I/O0

33    -   I/O1

35    -   I/O2

37    -   I/O3

39    -   I/O4

41    -   I/O5

43    -   I/O6

45    -   I/O7


### Signals

5   -     WED

2   -     OED

4   -     CED


Power of the EEPROM is from Arduino  PWR 5V and GND

