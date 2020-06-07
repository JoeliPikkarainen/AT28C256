
/*Project to program AT28C256 EEPROM
  with Arduino Mega

*/

#include <avr/io.h>

//For setting The 15 ADDRESS pins on the EEPROM
const uint8_t ADDR[] = {20,22,24,26,28,30,32,34,36,38,40,42,44,46,48};
#define NUM_ADDRS 15

//For data I/O on the EEPROM
const uint8_t DATA[] = {31,33,35,37,39,41,43,47};
#define NUM_DATAS 8

//WED Write Enable on the EEPROM
#define EEPROM_SIZE (32 * 1000)
#define WED 5
#define OED 2
#define CED 4
#define BIT_AT(D,A) ((D >> A) & 0x01)
#define DELAY_MS 0

void setup() {

  Serial.begin(115200);
}

void loop() {

  write_read_test(0xFF);
}

void write_read_test(uint8_t data_in){
  /*
  Write EEPROM full of bytes data_in

  */

  uint8_t data_out;
  for(int i = 0; i < EEPROM_SIZE;i++){

    data_out = 0;

    char str_address[30];
    sprintf(str_address,"Address: 0x%04X ",i);
    Serial.print(str_address);
    Serial.print("W -> ");
    Serial.print(data_in);  
    write_byte(data_in,i);

    read_byte(data_out,i);
    Serial.print(" R <- ");
    Serial.println(data_out);

    delay(0);
    }

    
}

void write_byte(uint8_t data, uint16_t addr){

    //Set the write ADDRESS

    //Set for output
    for(int i; i < NUM_ADDRS;i++){
      pinMode(ADDR[i],OUTPUT);
      }

    //Set the write address on the parallel I/O
    for(int i = 0; i < NUM_ADDRS;i++){
        bool thebit = BIT_AT(addr,i);
        digitalWrite(ADDR[i], thebit);
      }

    //Set the write DATA
    //Set for output
    for(int i; i < NUM_DATAS;i++){
        pinMode(DATA[i],OUTPUT);
      }

    //Set the write data on the parallel I/O
     for(int i = 0; i < NUM_DATAS;i++){
        bool thebit = BIT_AT(data,i);
        digitalWrite(DATA[i], thebit);
      }

   delay(DELAY_MS);
   EEPROM_put_addr();
   delay(DELAY_MS);
   EEPROM_SET_I();
   delay(DELAY_MS);
  
}


void read_byte(uint8_t &data, uint16_t addr){
  
     //Set the read ADDRESS

    //Set for output
    for(int i; i < NUM_ADDRS;i++){
      pinMode(ADDR[i],OUTPUT);
      }

    //Set the write address on the parallel I/O
    for(int i = 0; i < NUM_ADDRS;i++){
        bool thebit = BIT_AT(addr,i);
        digitalWrite(ADDR[i], thebit);
      }

    //Set the read DATA
    //Set for input
    for(int i; i < NUM_ADDRS;i++){
        pinMode(DATA[i],INPUT);
      }

    //Set the get data on the parallel I/O
     for(int i = 0; i < NUM_DATAS;i++){
        bool thebit = BIT_AT(data,i);
        digitalWrite(DATA[i], thebit);
      }
      
    EEPROM_SET_O();
    delay(DELAY_MS);
    
    //Read the I/O pins
    data = 0;
    for(int i = 0; i < NUM_DATAS;i++){
      bool thebit = digitalRead(DATA[i]);
      data = data | (thebit <<i);
      }

/*
     Serial.print("R ADDRESS:");
     Serial.print(addr);

     Serial.print(" R DATA:");
     Serial.print(data);
     */
}

void set_output_enable(){
  digitalWrite(WED,0);
}

void EEPROM_SET_I(){
  //Latch data, start write cycle  
  digitalWrite(WED,1);
  digitalWrite(OED,1);
}

void EEPROM_put_addr(){
  //Latch address
  digitalWrite(WED,0);
  digitalWrite(CED,0);
}

void EEPROM_SET_O(){
  //Asserts data from the address to I/O pins
  digitalWrite(CED,0);
  digitalWrite(OED,0);
  digitalWrite(WED,1);  
}
