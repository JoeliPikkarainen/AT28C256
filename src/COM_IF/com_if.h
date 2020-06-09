#ifndef COM_IF_H
#define COM_IF_H
#include <Arduino.h>

#define PRINT_BLOCK 7

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

    const char* cmd_flash_file  =   "flash-file\n";
    const char* cmd_read_EEPROM =   "read-eeprom\n";
    const char* cmd_help        =   "help\n";
    const char* cmd_write_test  =   "write-test\n";  

private:


};
#endif //COM_IF_H