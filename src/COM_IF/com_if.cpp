#include "com_if.h"
#include <Arduino.h>
#include "../EEPROM_UTILS/EEPROM_UTILS.h"

COM_IF::COM_IF()
{


}

void COM_IF::get_cmd(){
    char command[255];
    
    while(!Serial.available());
    delay(1000);

    int i = 0;
    while(Serial.available()){
      
      command[i] = Serial.read();
      i++;

    }
    
    command[i] = 0;
    Serial.println("EEPROM-FLASHER: Got command:");
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

  else if(strcmp((const char*)cmd_flash_rst_v,(const char*)command) == 0){
    flash_reset_vector();  
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
  delay(1);
  Serial.println("Flashing...");

  while(Serial.available() > 0 || addr < EEPROM_SIZE - 1){

    //Wait until Serial if has lsb and msb ready
    while(Serial.available() < 2);

    uint8_t lsb = Serial.read();
    uint8_t msb = Serial.read();

    
    write_byte(msb,addr);
    delay(10);

    write_byte(lsb,addr+1);
    delay(10);

    uint8_t msb_out;
    uint8_t lsb_out;
    read_byte(msb_out,addr);
    delay(10);

    read_byte(lsb_out,addr+1);
    delay(10);


    
    if(msb != msb_out || lsb != lsb_out){
          Serial.println("Error during flashing!!! verification un-match");
          char err_out[64];
          sprintf(err_out, "    at address 0x%04X\n",addr);
          Serial.println(err_out);
          return;
    }
    else{
        char report_out[64];

        sprintf(report_out,"0x%04X %02X", addr -1 ,msb);
        Serial.println(report_out);

        sprintf(report_out,"0x%04X %02X", addr ,lsb);
        Serial.println(report_out);

       }

#ifdef REPORT_OUT
    char report_progress[16];
    sprintf(report_progress,"%d / %d\n",addr,EEPROM_SIZE);
    Serial.println(report_progress); 
#endif




    Serial.println(addr);
    addr+=2;
  }
    Serial.print("done");
    delay(200);
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

    if(address > EEPROM_SIZE){
      Serial.println("Error max address is 0x8000 ");
      return;
    }

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
        if(n > EEPROM_SIZE){
          Serial.println("\n Maximum address reached, quiting..");
          return;
        }

    }
    Serial.println("\nDONE");
}

void COM_IF::help_msg()
{
    Serial.println("EEPROM-FLASHER");
    Serial.println("----COMMANDS----");

    Serial.println("flash-file");
    Serial.println("read-eeprom");
    Serial.println("write-test");
    Serial.println("flash-reset-vector");
    Serial.println("help");

}
 void COM_IF::run_write_test(){
    uint8_t out;
   
    uint8_t data_in[8] = { 0x00, 0x01,0x02 ,0x03,0x04,0x05,0x06,0x07};

    //uint8_t data_in[8] = { 0x00, 0x01,0x00 ,0x00,0x00,0x00,0x00,0x00};

    write_test(8,data_in);
    Serial.println("-------");

    read_test(8,&out);
    Serial.println("-------");

    delay(1000);
    Serial.println("Done");

 }
void COM_IF::flash_reset_vector(){

    Serial.println("Flashing reset vector...");
    uint8_t reset_vector[4] { 0x80, 0x00 , 0x00, 0x00};
    uint16_t start_address = EEPROM_SIZE - 4;
    for(int i = 0; i < 4 ;i++){
      write_byte(reset_vector[i],start_address+i);
      delay(10);
    }
    Serial.println("DONE");
}
