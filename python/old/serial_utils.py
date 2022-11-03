import serial, string, time, sys, os


class SerialUtils:

	def __init__(self,serial):
		self.serial = serial

	def prnt(self,to_out):
		sys.stdout.write(str(to_out))
		sys.stdout.flush()


	def single_msg_r(self,ser):
		output = " "
		output_decoded = " "
		first_time = True

		while (ser.inWaiting() == 0):
			time.sleep(0.01)

		time.sleep(0.5)

		output = ser.readline()
		output_decoded = output.decode()
		self.prnt("MSG:")
		self.prnt("")
		self.prnt(output_decoded)
		self.prnt("")


	def single_msg_r_old(ser):
		output = " "
		output_decoded = " "
		first_time = True

		while (len(output_decoded) != 0):
			if first_time == False:
				self.prnt(self,"Got message of length:")
				self.prnt(self,len(output))
				self.prnt(self,'\n')
			else:
				first_time = False

			output = ser.readline()
			output_decoded = output.decode()
			if output == "b''\n":
				break

			self.prnt(output_decoded)