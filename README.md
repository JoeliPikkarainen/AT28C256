# AT28C256
This is project to program AT28C256 EEPROM with ATmega 2560

DONE: Working write and read functions

DONE: Working test function to fill EEPROM with single value and verify

DONE: Fixed a bug caused by wrong ADDRESS pin setup, write, read works now

TODO: Add possibility send .hex file from Serial Monitor to Arduino and program the eeprom

TODO: Make COM_IF for Arduino, needed functions: 

void set_start_address(uint16_t address)

bool program(char * hexfile,bool verify)

bool verify(char* hexfile, uint16_t start_address)


## Connections Arduino to AT28C256

### Address

22    -   A0

24    -   A1

26    -   A2

28    -   A3

30    -   A4

32    -   A5

34    -   A6

36    -   A7

38    -   A8

40    -   A9

42    -   A10

44    -   A11

46    -   A12

48    -   A13

50    -   A14

52    -   NC

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

