import serial, string, time, sys, os

sys.path.append('.')
import command_handler as ch
import serial_utils as su

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
			serial.prnt(output)
	inp = input()

	if inp == "pyfuncs":
		py_funcs()
	elif inp == "read-eeprom":
		ch.handle_read_eeprom(ser)
	else:
		inpendl = inp+'\n'
		ser.write(bytes(inpendl.encode()))
		use_serial()

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

	for i in range(len(file_data)):
		byte1 = file_data[i]
		byte2 = file_data[i+1]
		prnt("addr %d data %d \n" % (i,byte1))
		prnt("addr %d data %d \n" % (i,byte2))

		print()
		ser.write(byte1)
		ser.write(byte2)

		#Block until 2 written bytes are echoed back
		while ser.in_waiting < 2:
			time.sleep(0.01)

		byte1e = ser.read(1)
		byte2e = ser.read(1)

		prnt("e addr %d data %d" % (i,byte1e))
		prnt("e addr %d data %d" % (i,byte2e))

	print("File flashing done!")
	use_serial()



print ("----------")
print ("Python Serial Commander for project AT28C256")
print ("sources: https://github.com/JoeliPikkarainen/AT28C256")
print ("use \"pyfuncs\" to enter PC command interface")
print ("Connecting to tty device....")
print ("----------")

if os.path.exists("/dev/ttyACM0"):
	ser = serial.Serial('/dev/ttyACM0',115200,8,'N',1,timeout=1)
	serial = su.SerialUtils(ser)
else:
	print("No device found at \"/dev/ttyACM0\"... quiting")
	sys.exit(1)

EEPROM_SIZE = 32768

ser.write("help\n".encode())
use_serial()
