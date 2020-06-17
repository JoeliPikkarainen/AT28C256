#include <Arduino.h>
#include <avr/io.h>
#include "EEPROM_UTILS/EEPROM_UTILS.h"
#include "COM_IF/com_if.h"

#define V_MAJOR 0
#define V_MINOR 0
#define V_PATCH 0

void main_loop(void);
void entry_point(void);
void welcome();

void setup(){}
void loop(){entry_point();}
COM_IF m_com;

void entry_point(){


  //Init
  Serial.begin(115200);


  welcome();
  m_com.help_msg();

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

void welcome(){

  Serial.println("EEPROM flasher for AT28C256");
  char tmp_intro[127];
  sprintf(tmp_intro,"Version: %d.%d.%d",V_MAJOR,V_MINOR,V_PATCH);
  Serial.println(tmp_intro);
  Serial.println("Joeli Pikkarainen Finland 2020");
  Serial.println("https://github.com/JoeliPikkarainen/AT28C256");

}