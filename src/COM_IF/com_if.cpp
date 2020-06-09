#include "com_if.h"
#include <Arduino.h>
#include "../EEPROM_UTILS/EEPROM_UTILS.h"

COM_IF::COM_IF()
{


}

void COM_IF::get_cmd(){
    char command[255];
    
    while(!Serial.available());
    delay(100);

    int i = 0;
    while(Serial.available()){
      
      command[i] = Serial.read();
      //Serial.print(command[i]);
      i++;

    }
    
    command[i] = 0;
    Serial.print(command);

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

  /*
    The program decodes ASCII string to bytes:
    [char][char][space-separator] -> [byte]
    ...
    example
    [F][F][ ] -> [FF]
  

  
  */

  Serial.println("waiting for hex file");
  uint16_t addr = 0;
  while(Serial.available() < 1);
  delay(500);
  Serial.println("Flashing...");
  uint8_t new_line_ctr = 0;

  while(Serial.available() > 0){

    //Take first byte from ASCII
    char byte_str[2];

    byte_str[0] = Serial.read();
    byte_str[1] = Serial.read();


    uint8_t byte_to_flash = strtol((const char*)byte_str,nullptr,16);
    write_byte(byte_to_flash,addr);
    
    delay(10);
    uint8_t data_out;
    read_byte(data_out,addr);

    char str_out[64];
    sprintf(str_out," 0x%04X 0x%02X vs 0x%02X ->",addr,byte_to_flash,data_out);
    Serial.print(str_out);

    if(byte_to_flash != data_out){
          Serial.println();
          Serial.println("Error during flashing!!! verification un-match, quitting");
          Serial.println("    at address:");
          Serial.print(str_out);
          return;
    }
    //Read the incoming space away
    Serial.print (Serial.read());
    Serial.println();
    addr++;
    
    delay(0);
  }
    char str_out[64];
    sprintf(str_out,"Done %d bytes flashed",addr);
    Serial.println(str_out);

}

void COM_IF::read_from_EEPROM(){

    //Wait for parameter [ADDRES]
    Serial.println("Give start address (dec) or (0xhex)");
    while(Serial.available() < 1);
    delay(500);
    char address_str[50];
    int i = 0;
    while(Serial.available() > 0){
        address_str[i] = Serial.read();
        i++;
    }

    uint16_t address = (uint16_t)strtol(address_str, NULL, 0);

    char tmp_msg[64];
    sprintf(tmp_msg,"Got address: 0x%04X",address);
    Serial.println(tmp_msg);

    Serial.println("How many bytes to read? (dec) or (0xhex)");
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
    Serial.println("write-test");
    Serial.println("help");

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
