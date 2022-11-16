import serial, string, time, sys, os, struct, time, binascii, subprocess, datetime

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


def unhex_filedata(start, length, filedata):
	HexString = ""
	i = 0
	
	if length > len(filedata)-1:
		length = len(filedata)-1

	thepart = []
	for r in range(length):
		addr = start + r
		if addr > len(filedata) -1:
			break;
		thepart.append(filedata[addr])

	for digt in thepart:
				HexVal = hex(digt)[2:]
				if len(HexVal) == 1:
					HexString += '0' + HexVal
				else:
					HexString += HexVal

				if i >= length:
					break

				i+=1
				

	return binascii.unhexlify(HexString)
	
def py_funcs():
	print("Python methods:")
	print("1. py-flash-file")
	print("2. py-flash-file-bulk-bin")
	print("3. py-read-to-file-bulk-bin")

	inp = input()

	if inp == "py-flash-file" or inp == "1" :
		py_flash_file(1)
	if inp == "py-flash-file-bulk-bin" or inp == "2" :
		py_flash_file(2)
	if inp == "py-read-to-file-bulk-bin" or inp == "3" :
		py_read_file()
	else:
		print("Command not recognized")
		print("Back to serial...")
		use_serial()


def py_flash_file(style):
	print("path to file:")
	filepath = input()
	file = open(filepath,"rb")
	hex_data = file.read(EEPROM_SIZE)
	if style == 1:
		py_internal_do_flash(hex_data)
	elif style == 2:
		py_internal_do_flash_bulk_bin(hex_data)

	#Verify?
	dt = datetime.datetime.now()    # for date and time
	ts = datetime.datetime.timestamp(dt)    # for timestamp
	verfilepath = filepath +  ".verify." + str(ts)
	verfsize = len(hex_data)
	verfstart = 0
	py_internal_read_to_file(verfilepath,verfstart, verfsize)
	md5sum_compare(filepath,verfilepath)
	use_serial()


def py_internal_do_flash(file_data):
	prnt("py_internal_do_flash: size bytes %d : \r\n" % (len(file_data)))
	#ser.write(bytes("flash-file\n".encode()))
	time.sleep(2)
	resp = ser.readline()
	prnt(resp.decode)
	time.sleep(2)
	sequencer = 0
	sequencer_limit = 6
	precenter = 0;
	errorCounter = {}
	timerBegin = time.perf_counter()
	for i in range(len(file_data)):
		byte = file_data[i]
		byteStr = ("wta\n%d %d\n" % (i,byte))
		prnt("Writing serial\r\n")
		ser.write(bytes(byteStr.encode()))
		sequencer+=1
		if sequencer > sequencer_limit:
			#prnt("\nSequencer")
			time.sleep(0.0001)
			sequencer = 0
			output = ' '
			while output != "":
				output = ser.readline()
				output = output.decode()
				prnt(output)
				if "MISSMATCH" in output or "Invalid" in output:
					errorCounter.push(i)


		precenter = (i / len(file_data)) * 100
		timerEnd = time.perf_counter()
		tookSeconds = timerEnd - timerBegin
		prnt("prog: %f %% errors:%d time: %f\n\r" % (precenter,len(errorCounter),tookSeconds))

	timerEnd = time.perf_counter()
	tookSeconds = timerEnd - timerBegin
	prnt("Finished with %d error, took %f seconds\r\n" % (len(errorCounter),tookSeconds))
	use_serial()
	
def py_internal_do_flash_bulk_bin(file_data):
	cmd1 = "wta-bulk-bin\n"
	cmd2 = "%d %d %d\n" % (0,len(file_data),0)
	ser.write(bytes(cmd1.encode()))
	ser.write(bytes(cmd2.encode()))

	time.sleep(1)
	resp = ser.readline()
	prnt(resp.decode())

	time.sleep(1)
	resp = ser.readline()
	prnt(resp.decode())

	time.sleep(1)

	prnt("\n")
	errorCounter = {}
	timerBegin = time.perf_counter()
	filedata = bytearray(file_data)

	prnt("py_internal_do_flash_bulk_bin: size bytes %d : \r\n" % (len(filedata)))
	prnt("\r\n")
	seq = 0
	prnt("R\r\n")
	hexbulk = 8
	j = 0
	for i in range(len(filedata)):
		#time.sleep(0.5)
		unhexedData = unhex_filedata(j,hexbulk,filedata)
		prnt("Data >> %d" % len(unhexedData))
		prnt(unhexedData)
		ser.write(unhexedData)
		prnt("W\r\n")
		output = ""
		while not "E" in output:
			output = ser.readline()
			output = output.decode(encoding = "ISO-8859-1")
			if output != "":
				prnt(output)
				prnt("\r\n")
			else:
				prnt("Waiting for continue signal\r\n")
			#time.sleep(0.5)

		prnt(output)
		prnt("\r\n")
		prnt("%f %%\r\n" % ( (j  / len(filedata)) * 100 ))
		j += 8
		if j >= len(filedata)-1:
			prnt("Break, all data flashed")
			break

	timerEnd = time.perf_counter()
	tookSeconds = timerEnd - timerBegin
	prnt("Finished with %d error, took %f seconds\r\n" % (len(errorCounter),tookSeconds))

def py_read_file():
	print("path to file:")
	filepath = input()

	print("start address:")
	start = input()

	print("file size:")
	fsize = input()

	py_internal_read_to_file(filepath,start,fsize)
	use_serial()

def py_internal_read_to_file(filename, startadrr, fsize):
	f = open(filename, "wb")
	f.close()
	f = open(filename, "ab")

	binread_cmd = "rfa-bulk-bin %d %d\n" % ( int(startadrr), int(fsize)) 
	ser.write(bytes(binread_cmd.encode()))

	prnt("Waiting response\r\n");

	cmd_response = ""
	time.sleep(1)
	while not "read_addr_bulk_bin" in cmd_response:
		cmd_response = ser.readline().decode()
		if cmd_response != "":
			print(cmd_response)


		
	prnt("\r\n------------------------------\r\n")
	prnt("\r\nGot Response:\r\n")
	print(cmd_response)
	prnt("incoming data to file...\r\n")
	prnt("\r\n------------------------------\r\n")

	filebuff = []
	while len(filebuff) < int(fsize):
		serdata = ""
		serdata = ser.read(1)
		if serdata == "":
			continue

		if len(serdata) != 1:
			continue

		tofile = struct.unpack('c', serdata)[0]
		filebuff.append(tofile)

	for b in filebuff:
		bu = (b)
		f.write(bu)

	f.close()
	prnt("FILE READING DONE\r\n")
	return len(filebuff)

def md5sum_compare(file1, file2):
	cmd1 = 'md5sum ' + file1
	cmd2 = 'md5sum ' + file2
	hash1 = subprocess.check_output(args=cmd1, shell=True).decode().split(' ')[0]
	hash2 = subprocess.check_output(args=cmd2, shell=True).decode().split(' ')[0]

	prnt("Check sums:\r\n")
	prnt(hash1 + " " + file1)
	prnt("\r\n")
	prnt(hash2 + " " + file2)
	prnt("\r\n")

	matched = hash1 == hash2

	if not matched:
		prnt("FILES DIFFER:\r\n")
		#xxd file1 > file1.txt
 		#xxd file2 > file2.txt
		file1txtname = file1  + ".txt"
		file2txtname = file2  + ".txt"

		file1txtcmd = "xxd " + file1 + " > " + file1txtname
		file2txtcmd = "xxd " + file2 + " > " + file2txtname

		subprocess.check_output(args=file1txtcmd, shell=True)
		subprocess.check_output(args=file2txtcmd, shell=True)

		diffcmd = "diff " + file1txtname + " " + file2txtname
		prnt(diffcmd + "\r\n")
		diffoutput = subprocess.getoutput(diffcmd)
		prnt(diffoutput)
		prnt("\r\n")

	return matched



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
