import serial, string, time, sys, os
import serial_utils


def handle_read_eeprom(ser):
	endl = "\n".encode()

	#Send command
	command = "read-eeprom\n"
	ser.write(command.encode())
	ser.flush()

	seri_uti = serial_utils.SerialUtils(ser)
	seri_uti.single_msg_r(ser)#"EEPROM-FLASHER: Got command:"
	seri_uti.single_msg_r(ser)#the command

	seri_uti.single_msg_r(ser)#"Give start address (dec) or (0xhex)\n"

	addr = input()
	addr = str(str(addr).encode() + endl).encode()
	#Send start Address
	ser.write(addr)
	ser.flush()

	#Get response
	seri_uti.single_msg_r(ser) #    sprintf(tmp_msg,"Got address: 0x%04X \n",address);
	time.sleep(1)
	seri_uti.single_msg_r(ser) # "How many bytes to read? (dec) or (0xhex)\n"

	num_bytes = int(input())

	ser.write(str(num_bytes).encode())
	ser.flush()

	ser.write(endl)
	ser.flush()

	seri_uti.single_msg_r(ser) # "Starting...\n"
	
	print("DATA:")
	for i in range(num_bytes):
		b = int(ser.read(1))
		ashex = hex(b)
		asint = int(ashex,0)
		print(ashex)
		print(asint)
		
		print("----")



	print("handle_read_eerpom done")
