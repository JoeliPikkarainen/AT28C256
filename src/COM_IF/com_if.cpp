#include "com_if.h"
#include <Arduino.h>
#include "../EEPROM_UTILS/EEPROM_UTILS.h"

COM_IF::COM_IF()
{


}

void COM_IF::get_cmd(){
    char command[255];
    
    if(Serial.available() > 1){
    delay(10);

    int i = 0;
    while(Serial.available()){
      
      command[i] = Serial.read();
      //Serial.print(command[i]);
      i++;
      }
      command[i] = 0;
      Serial.print(command);

    }
    else{
      return;
      }

  //Command to flash incoming file
  if(strcmp((const char*)cmd_flash_file,(const char*)command) == 0){
    wait_for_file();  
  }

    //Command to read from eeprom
  else if(strcmp((const char*)cmd_read_EEPROM,(const char*)command) == 0){
    read_from_EEPROM();  
  }
    //Command to print help message
  else if(strcmp((const char*)cmd_help,(const char*)command) == 0){
    help_msg();  
  }

  else if(strcmp((const char*)cmd_write_test,(const char*)command) == 0){
    run_write_test();  
  }


  else{
    
    Serial.println("Invalid Command");
    }
    return;
}

void COM_IF::wait_for_file(){
  Serial.println("waiting for hex file");
  uint16_t addr = 0;
  while(Serial.available() < 1);
  delay(500);
  Serial.println("Flashing...");

  while(Serial.available() > 0){

    uint8_t data = Serial.read();
    write_byte(data,  addr);

    char tmp_out[16];
    sprintf(tmp_out,"0x%04X 0x%02X ",addr,data);
    Serial.println(tmp_out);


    addr++;
  }
    char str_out[64];
    sprintf(str_out,"Done %d bytes flashed",addr);
    Serial.println(str_out);

}

void COM_IF::read_from_EEPROM(){

    //Wait for parameter [ADDRES]
    Serial.println("Give start address (hex)");
    while(Serial.available() < 1);
    delay(500);
    char address_str[50];
    int i = 0;
    while(Serial.available() > 0){
        address_str[i] = Serial.read();
        i++;
    }

    uint16_t address = (uint16_t)strtol(address_str, NULL, 0);
    Serial.print("Got address:");
    Serial.println(address);

    Serial.println("How many bytes to read? (hex)");
    while(Serial.available() < 1);
    delay(500);

    i = 0;
    char bytes_to_read_str[50];
    while(Serial.available() > 0){
        bytes_to_read_str[i] = Serial.read();
        i++;
    }
    uint16_t bytes = (uint16_t)strtol(bytes_to_read_str, NULL, 0);
    Serial.print("Bytes to read:");
    Serial.println(bytes);

    //Do the reading
    uint8_t block_ctr = 0;
    uint16_t end_addr = address + bytes;
    for(uint16_t n = address; n < end_addr;){

        if(block_ctr == 0){
            char tmp_block[16];
            sprintf(tmp_block,"0x%04X : ",n);
            Serial.print(tmp_block);
        }
        uint8_t data;
        char tmp_data[16];
        read_byte(data,n);
        sprintf(tmp_data," 0x%02X ",data);
        Serial.print(tmp_data);

        block_ctr++;
        if(block_ctr > PRINT_BLOCK){
            block_ctr = 0;
            Serial.println();
        }

        n++;

    }
    Serial.println("\nDONE");
}

void COM_IF::help_msg()
{
    Serial.println("----COMMANDS----");

    Serial.println("flash-file");
    Serial.println("read-eeprom");
    Serial.println("help");
    Serial.println("write-test");
}
 void COM_IF::run_write_test(){
    uint8_t *out;    
    uint8_t data_in[8] = { 0x00, 0x01,0x02 ,0x03,0x04,0x05,0x06,0x07};

    //uint8_t data_in[8] = { 0x00, 0x01,0x00 ,0x00,0x00,0x00,0x00,0x00};

    write_test(8,data_in);
    Serial.println("-------");

    read_test(8,out);
    Serial.println("-------");

    delay(1000);
    Serial.println("Done");

 }
