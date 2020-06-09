#include <Arduino.h>
#include <avr/io.h>
#include "EEPROM_UTILS/EEPROM_UTILS.h"
#include "COM_IF/com_if.h"

void main_loop(void);
void entry_point(void);

void setup(){}
void loop(){entry_point();}
COM_IF m_com;

void entry_point(){

  //Init
  Serial.begin(115200);
  Serial.println("starting...");
  //Address is always OUTPUT
  for(int i; i < NUM_ADDRS;i++){
      pinMode(ADDR[i],OUTPUT);
  }

   pinMode(WED,OUTPUT);
   pinMode(OED,OUTPUT);
   pinMode(CED,OUTPUT);
   digitalWrite(OED,1);
   digitalWrite(CED,1);
   digitalWrite(WED,1);



  //Main Loop
  while(1){
    main_loop();
  }

  return;
}

void main_loop(){

//#define TEST_BLOCKS
//#define TEST_BYTES
//#define TEST_READ_ALL


#ifdef TEST_BYTES
  write_read_test(0x00);
#endif

#ifdef TEST_READ_ALL
  read_all();
#endif

//END OF TESTS - START ACTUAL PROGRAM

m_com.get_cmd();

}

