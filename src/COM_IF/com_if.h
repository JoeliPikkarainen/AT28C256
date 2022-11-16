#ifndef COM_IF_H
#define COM_IF_H
#include <Arduino.h>
#include <WString.h>

#define PRINT_BLOCK 7
//#define REPORT_OUT
class COM_IF
{



public: 
    COM_IF();
    ~COM_IF() {};

    void get_cmd();

    void wait_for_file();
    void read_from_EEPROM();
    void help_msg();
    void run_write_test();
    void flash_reset_vector();
	void write_to_addr();
	void write_to_addr_bulk_bin();

	void read_addr_bulk_bin_parse(String);
	void read_addr_bulk_bin(uint32_t start, uint32_t len);
	
    const char* cmd_flash_file  =   "flash-file\n";
    const char* cmd_read_EEPROM =   "read-eeprom\n";
    const char* cmd_help        =   "help\n";
    const char* cmd_write_test  =   "write-test\n";  
    const char* cmd_flash_rst_v =   "flash-reset-vector\n";
	const char* cmd_write_to_addr = "wta\n";
	const char* cmd_write_to_addr_bulk_bin = "wta-bulk-bin\n";
	const char* cmd_read_addr_bulk_bin = "rfa-bulk-bin";

private:


};
#endif //COM_IF_H