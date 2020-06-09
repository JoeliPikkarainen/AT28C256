
#include <Arduino.h>
#include <avr/io.h>
#include "EEPROM_UTILS.h"

#define NUM_ADDRS 15
#define NUM_DATAS 8

#define EEPROM_SIZE (32 * 1000)
#define WED 5   //Write Enable
#define OED 2   //Output Enable
#define CED 4   //Chip Enable
#define BIT_AT(D,A) ((D >> A) & 0x01)
#define DELAY_MS 0
#define DELAY_MICRO 0

void enableChip()   {digitalWrite(CED,0);}
void disableChip()  {digitalWrite(CED,1);}
void enableOutput() {digitalWrite(OED,0);}
void disableOutput(){digitalWrite(OED,1);}
void enableWrite()  {digitalWrite(WED,0);}
void disableWrite() {digitalWrite(WED,1);}

void write_byte(uint8_t data, uint16_t addr){
  
    disableOutput();
    disableWrite();
    
    //Set for output
    set_data_direction(OUTPUT); 
    //Set the write ADDRESS
    set_address(addr);
    
    //Set the write data on the parallel I/O
    set_data(data);


   enableChip();
   delayMicroseconds(DELAY_MICRO);
   
   enableWrite();
   delayMicroseconds(DELAY_MICRO);
   
   disableWrite();
   delayMicroseconds(DELAY_MICRO);

   //wait_write_finished(data);
   set_address(0);
   disableChip();
}


void read_byte(uint8_t &data, uint16_t addr){

    //set the read address
    set_address(addr);
    
    //Set for input read
    set_data_direction(INPUT);
    
    disableOutput();
    disableWrite();
    disableChip();
    delayMicroseconds(100);

    enableChip();
    //delayMicroseconds(DELAY_MICRO);

    enableOutput();   
    //delayMicroseconds(DELAY_MICRO);
    
    //Read the I/O pins
    data = 0;
    read_io(data);

    disableOutput();
    disableChip();

}

void set_data_direction(uint8_t mode){
  
  for(int i = 0; i < NUM_DATAS;i++){
    pinMode(DATA[i],mode);
    }
}


void set_data(uint8_t data){
  
   for(int i = 0; i < NUM_DATAS;i++){
      bool thebit = BIT_AT(data,i);
      digitalWrite(DATA[i], thebit);
    }
  
}

void read_io(uint8_t &data){
  
   for(int i = 0; i < NUM_DATAS;i++){
      bool thebit = digitalRead(DATA[i]);
      data = data | (thebit <<i);
    }
    
}

void wait_write_finished(uint8_t written){

 uint8_t first = 0;
 uint8_t second = 0;
 set_data_direction(INPUT);
  do
  {
    enableOutput();
    delayMicroseconds(DELAY_MICRO);
    read_io(first);
    disableOutput();
    enableOutput();
    delayMicroseconds(DELAY_MICRO);
    read_io(second);
    disableOutput();
    Serial.println(first);
    Serial.println(second);
    Serial.println(written);
    Serial.println();
    
    if((first == second)  || (first == written)){
        break;
      }
    first = 0;
    second = 0;
  }while(1);
  disableChip();
}

void write_read_test(uint8_t data_in){
  /*
  Write EEPROM full of bytes data_in

  */

  uint8_t data_out;
  for(int i = 0; i < EEPROM_SIZE;i++){

    data_out = 0;

    char str_out[30];
    sprintf(str_out,"Address: 0x%04X ",i);
    Serial.print(str_out);

    sprintf(str_out,"W -> 0x%02X",data_in);
    Serial.print(str_out);   
    write_byte(data_in,i);

    read_byte(data_out,i);
    sprintf(str_out," R <- 0x%02X",data_out);
    Serial.print(str_out);
    Serial.println();
    delay(100);
    data_in++;
    }

    
}

void write_all(){
    char str_out[30];
    uint8_t data_in = 0;

  for(int i = 0; i < EEPROM_SIZE;i++){
    write_byte(i,i);
    sprintf(str_out,"WRITE ALL: 0x%04X 0x%02X ",i,data_in);
    Serial.println(str_out);
    data_in++;
    } 
  }
void read_all(){
  char str_out[30];

  for(int i = 0; i < EEPROM_SIZE;i++){
    uint8_t data_out;
    read_byte(data_out,i);
    sprintf(str_out,"READ ALL: 0x%04X 0x%02X ",i,data_out);
    Serial.println(str_out);
    }
      
}
void write_test(int num_bytes,uint8_t* data){

  char str_out[30];
  uint16_t addr = 0;
  for(int i = 0; i < num_bytes;i++){
    
    sprintf(str_out,"Write test in: 0x%04X 0x%02X ",addr,data[i]);
    Serial.print(str_out);
    Serial.println();

    write_byte(data[i],addr);
    delay(1000);
    addr++;
    }
}

void read_test(int num_bytes,uint8_t* data){
  
  char str_out[30];
  uint8_t data_out1;
  uint8_t data_out2;
  uint16_t addr = 0;

  for(int i = 0; i < num_bytes;i++){
    
    data_out1 = 0;
    data_out2 = 0;
    
    read_byte(data_out1,addr);
    delay(10);
    read_byte(data_out2,addr);
    
    sprintf(str_out,"Read test out: 0x%04X 0x%02X vs 0x%02X ",addr,data_out1, data_out2);
    Serial.print(str_out);
    Serial.println();
    delay(1000);
    addr++;
    
    }
}


void set_address(uint16_t addr){
  
   for(int i = 0; i < NUM_ADDRS;i++){
       bool thebit = BIT_AT(addr,i);
       digitalWrite(ADDR[i], thebit);
      }
}
