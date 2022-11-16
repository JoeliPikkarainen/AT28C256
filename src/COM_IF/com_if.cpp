#include "com_if.h"
#include <Arduino.h>
#include "../EEPROM_UTILS/EEPROM_UTILS.h"

COM_IF::COM_IF()
{


}

void COM_IF::get_cmd(){
    char command[255];
    String cmdstring = "";

    while(!Serial.available());
    delay(25);

    int i = 0;
    while(Serial.available()){
      
      command[i] = Serial.read();
	  cmdstring += command[i];
	  i++;

	  if(command[i-1] == '\n'){
		break;
	  }

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
  else if(strcmp((const char*)cmd_write_to_addr,(const char*)command) == 0){
    write_to_addr();  
  }
  else if(strcmp((const char*)cmd_write_to_addr_bulk_bin,(const char*)command) == 0){
    write_to_addr_bulk_bin();  
  }
  else if(cmdstring.indexOf("rfa-bulk-bin") > -1){
    read_addr_bulk_bin_parse(cmdstring);
  }
  else{
	String invString = "Invalid Command:" + cmdstring;
    Serial.println(invString);
  }
    return;
}

void COM_IF::wait_for_file(){

  Serial.println("waiting for hex file");
  uint16_t addr = 0;
  while(Serial.available() < 1);
  delay(1);
  Serial.println("Flashing...");

  while(Serial.available() > 0 || addr < 100 - 1){

    //Wait until Serial if has lsb and msb ready
    while(Serial.available() < 2);

    uint8_t lsb = Serial.read();
    uint8_t msb = Serial.read();
   
    write_byte(msb,addr);
    delay(15);

    write_byte(lsb,addr+1);
    delay(15);

    uint8_t msb_out;
    uint8_t lsb_out;
    read_byte(msb_out,addr);
    delay(15);

    read_byte(lsb_out,addr+1);
    delay(15);


    
    if(msb != msb_out || lsb != lsb_out){
          Serial.println("Error during flashing!!! verification un-match");
          char err_out[64];
          sprintf(err_out, "    at address 0x%04X\n",addr);
          Serial.println(err_out);
          return;
    }
    else{
        char report_out[64];

        sprintf(report_out,">>0x%04X 0x%02X", addr,msb);
        Serial.println(report_out);

		sprintf(report_out,"<<0x%04X 0x%02X", addr,msb_out);
        Serial.println(report_out);

        sprintf(report_out,">>0x%04X 0x%02X", addr +1,lsb);
        Serial.println(report_out);

        sprintf(report_out,"<<0x%04X 0x%02X", addr +1,lsb_out);
        Serial.println(report_out);

       }

#ifdef REPORT_OUT
    char report_progress[16];
    sprintf(report_progress,"%d / %d\n",addr,EEPROM_SIZE);
    Serial.println(report_progress); 
#endif

    //Serial.println(addr);
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

    Serial.println("How many bytes to read? (dec) or (0xhex)\r\n");
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
	Serial.println("wta\n <ADDRESS> <BYTE>");

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

void COM_IF::write_to_addr(){
	//When this command is received, the next 2 lines should be encoded string of the bytes

	//Wait for data
	while(Serial.available() < 1);

	//read data
	String instring = Serial.readStringUntil('\n');
	String addrstring;
	String datastring;

	char tmpchar = '1';
	unsigned int idx = 0;
	while(idx < instring.length()){
		tmpchar = instring.charAt(idx);
		idx++;
		if(tmpchar == ' '){
			break;
		}
		addrstring.concat(tmpchar);
	}

	while(idx < instring.length()){
		tmpchar = instring.charAt(idx);
		idx++;
		if(tmpchar == '\n'){
			break;
		}
		datastring.concat(tmpchar);
	}

	//Decode data:<address><space><data><\n>
	
	uint8_t addres_u8 = (uint8_t)addrstring.toInt();
	uint8_t data_u8 = (uint8_t)datastring.toInt();
	write_byte(data_u8,addres_u8);

	char outinter[128];
	//sprintf(outinter,"wta: 0x%04X 0x%02X",addres_u8, data_u8);
	//Serial.println(outinter);
	delay(15);
	uint8_t dout;
	read_byte(dout,addres_u8);

	//Verify
	if(data_u8 != dout){
		char output[128];
		sprintf(output,"MISSMATCH A: 0x%04X D: 0x%02X 0x%02X",addres_u8,data_u8,dout);
		Serial.println(output);
	}

}

void COM_IF::write_to_addr_bulk_bin(){

	//Parameters:<START_ADDR> <LENGTH> <DO_VERIFY>

	//Wait for data
	while(Serial.available() < 1);
	String start_addr_s = Serial.readStringUntil(' ');
	String length_s = Serial.readStringUntil(' ');
	String do_verify_s = Serial.readStringUntil('\n');


	int start_addr = start_addr_s.toInt();
	unsigned int length = length_s.toInt();
	int do_verify = do_verify_s.toInt();

	char outbuff[128];
	sprintf(outbuff,"write_to_addr_bulk_bin start:%d len:%d veriy:%d",start_addr,length,do_verify);
	Serial.println(outbuff);


	//The next data will be off binary writables
	int errors = 0;
	int writesDone = 0;
	int roundIdx = 0;
	for(int i = start_addr; writesDone < length && writesDone < EEPROM_SIZE;){
		while(Serial.available() < 1);
		char buff[SERIAL_RX_BUFFER_SIZE];
		int isAvail = Serial.available();
		isAvail = isAvail > SERIAL_RX_BUFFER_SIZE ? SERIAL_RX_BUFFER_SIZE : isAvail;
		int bytesReady = Serial.readBytes(&buff[0],isAvail);

		//Write part
		roundIdx = i;
		for(int j = 0; j < bytesReady && writesDone < length; j++){
			write_byte(buff[j],roundIdx);
			delay(15);
			writesDone++;
			roundIdx++;
		}

		//Verify part
		roundIdx = i;
		for(int j = 0; j < bytesReady; j++){
			uint8_t out;
			read_byte(out,roundIdx);
			delay(15);
			if(buff[j] != out){
				if(do_verify){
					errors++;
				}
			}
			i++;
			roundIdx++;
		}
		sprintf(outbuff,"ERROR:%d\r\n",errors);
		Serial.print(outbuff);
	}

	sprintf(outbuff,"Errors %d From: %d",errors,writesDone);
	Serial.println(outbuff);

}

void COM_IF::read_addr_bulk_bin(uint32_t start, uint32_t len){
	
	for(uint32_t i = 0; i < len; i++){
		uint8_t data;
		read_byte(data, start + i);
		Serial.write(data);
	}

	Serial.println("read_addr_bulk_bin done");
}

void COM_IF::read_addr_bulk_bin_parse(String cmd){

	String parts[4];
	int part = 0;
	for(int i = 0; i < cmd.length(); i++){
		char c = cmd.charAt(i);
		if(c == '\n' || c == ' '){
			part++;
			continue;
		}
		parts[part] += c;
	}

	uint32_t start_addr = (uint32_t)parts[1].toInt();
	uint32_t len = (uint32_t)parts[2].toInt();

	char out[127];
	sprintf(out, "Starting read_addr_bulk_bin %d %u\r\n",start_addr,len);
	Serial.print(out);
	read_addr_bulk_bin(start_addr,len);
}


