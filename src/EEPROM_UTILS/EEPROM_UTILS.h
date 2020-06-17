#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include <Arduino.h>
#include <avr/io.h>

//For setting The 15 ADDRESS pins on the EEPROM
const uint8_t ADDR[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50};
#define NUM_ADDRS 15

//For data I/O on the EEPROM
const uint8_t DATA[] = {31,33,35,37,39,41,43,45};
#define NUM_DATAS 8

#define EEPROM_SIZE 0x8000
#define WED 5   //Write Enable
#define OED 2   //Output Enable
#define CED 4   //Chip Enable
#define BIT_AT(D,A) ((D >> A) & 0x01)
#define DELAY_MS 0
#define DELAY_MICRO 0


void enableChip();
void disableChip();
void enableOutput();
void disableOutput();
void enableWrite();
void disableWrite();

void set_data_direction(uint8_t mode);
void set_address(uint16_t address);
void wait_write_finished(uint8_t data);
void read_io(uint8_t &data);
void set_data(uint8_t data);

void write_byte(uint8_t data, uint16_t addr);
void read_byte(uint8_t &data, uint16_t addr);
void write_test(int num_bytes,uint8_t* data);
void read_test(int num_bytes,uint8_t* data);




#endif