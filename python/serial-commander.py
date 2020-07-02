import serial, string, time, sys, os

def prnt(to_out):
	sys.stdout.write(str(to_out))
	sys.stdout.flush()

def use_serial():
	#print("Use serial")
	output = ' '
	while output != "":
		output = ser.readline()
		output = output.decode()
		if output != "b''\n":
			prnt(output)
			#print(output)
	inp = input()

	if inp == "pyfuncs":
		py_funcs()
	else:
		inpendl = inp+'\n'
		#print(inpendl)
		ser.write(bytes(inpendl.encode()))
		use_serial()



def py_funcs():
	print("Python methods:")
	print("py-flash-file")

	inp = input()

	if inp == "py-flash-file":
		py_flash_file()
	else:
		print("Command not recognized")
		print("Back to serial...")
		use_serial()


def py_flash_file():
	print("path to file:")
	filepath = input()

	file = open(filepath,"rb")

	hex_data = file.read(EEPROM_SIZE)
	py_internal_do_flash(hex_data)

def py_internal_do_flash(file_data):
	prnt("py_internal_do_flash")
	ser.write(bytes("flash-file\n".encode()))
	time.sleep(2)
	resp = ser.readline()
	prnt(resp.decode)
	time.sleep(2)
	sequencer = 0
	sequencer_limit = 64

	for i in range(len(file_data)):
		byte = file_data[i]
		#prnt(byte)
		prnt("addr %d data %d" % (i,byte))
		print()
		ser.write(byte)
		sequencer+=1
		if sequencer > sequencer_limit:
			prnt("\nSequencer")
			sequencer = 0
			output = ' '
			while output != "":
				output = ser.readline()
				output = output.decode()
				prnt(output)
				if output == "done":
					return 0
			time.sleep(0.001)

	use_serial()



print ("----------")
print ("Python Serial Commander for project AT28C256")
print ("sources: https://github.com/JoeliPikkarainen/AT28C256")
print ("use \"pyfuncs\" to enter PC command interface")
print ("Connecting to tty device....")
print ("----------")

if os.path.exists("/dev/ttyACM0"):
	ser = serial.Serial('/dev/ttyACM0',115200,8,'N',1,timeout=1)
else:
	print("No device found at \"/dev/ttyACM0\"... quiting")
	sys.exit(1)

EEPROM_SIZE = 32768

ser.write("help\n".encode())
use_serial()
