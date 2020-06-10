import serial, string, time, sys


def prnt(to_out):
	sys.stdout.write(str(to_out))
	sys.stdout.flush()

def use_serial():
	output = ' '
	while output != "":
		output = ser.readline()

		if output != 0:
			prnt(output)
	inp = raw_input()

	if inp == "pyfuncs":
		py_funcs()
	else:
		inpendl = inp+'\n'
		ser.writelines(inpendl)
		use_serial()



def py_funcs():
	print("Python methods:")
	print("py-flash-file")

	inp = raw_input()

	if inp == "py-flash-file":
		py_flash_file()
	else:
		print("Command not recognized")
		print("Back to serial...")
		use_serial()


def py_flash_file():
	print("path to file:")
	filepath = raw_input()

	file = open(filepath,"rb")

	hex_data = file.read(EEPROM_SIZE)
	py_internal_do_flash(hex_data)

def py_internal_do_flash(file_data):
	print("py_internal_do_flash")
	ser.writelines("flash-file\n")
	time.sleep(1)
	print(ser.readline())
	
	sequencer = 0
	sequencer_limit = 64

	for i in range(len(file_data)):		
		byte = file_data[i]
		prnt(byte)
		ser.writelines(byte)
		sequencer+=1
		if sequencer > sequencer_limit:
			print("\nSequencer")
			sequencer = 0


			output = ' '
			while output != "":
				output = ser.readline()
				prnt(output)
				if output == "done":
					return 0
			#time.sleep(0.000)

	use_serial()

ser = serial.Serial('/dev/ttyACM0',115200,8,'N',1,timeout=1)
EEPROM_SIZE = 36000

print ("----------")
print ("Python Serial Commander for project 28C256")
print ("use \"pyfuncs\" to enter PC command interface")
print ("Connecting to tty device....")
print ("----------")


use_serial()
