#!/usr/bin/python
# -*- coding: utf-8 -*-

## Tool to read tachograph data from a vehicle unit connected serially 
## Copyright(C) Andreas Gölzer

#This is free software: you can redistribute it and/or modify it under the
#terms of the GNU General Public License as published by the Free Software
#Foundation, either version 3 of the License, or (at your option) any later
#version.

#It is distributed in the hope that it will be useful, but WITHOUT ANY
#WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

#You should have received a copy of the GNU General Public License along with
#this program.  If not, see <http://www.gnu.org/licenses/>.

from serial import Serial;
from optparse import OptionParser;

parser = OptionParser(version="%prog 0.1")
parser.add_option("-s", "--serial", dest="serial",
                  help="serial terminal the VU is connected to", default='/dev/ttyUSB0', metavar="FILE")
parser.add_option("-o", "--output", dest="output",
                  help="File to save readesm data to", default='', metavar="FILE")


(options, args) = parser.parse_args()


class malformedHeader(Exception):
	def __init__(self, received):
		self.received = received
	def __str__(self):
		return "Expected 0x80 0xF0 0xEE, got " + self.received

class communicationError(Exception):
	def __init__(self, description):
		self.description = description
	def __str__(self):
		return description

class wrongChecksum(Exception):
	def __str__(self):
		return "Received message had wrong checksum"

class vuSerial:
	def __init__(self, maxBaudRate=115200):
		self.conn = Serial(options.serial, 9600, timeout=5)
		print "Sending 'Start Communication Request'"
		self.conn.write(chr(0x81) + chr(0xEE) + chr(0xF0) + chr(0x81) + chr(0xE0))
		response = self.singleResponse()
		print "Response is", response
		if response == chr(0xC1) + chr(0x8F) + chr(0xEA):
			print "Got 'Positive Response Start Communication Request'"
		else:
			raise communicationError("Could not establish communication, check connection")
		self.sendExpectingResponse(chr(0x10) + chr(0x81), chr(0x50) + chr(0x81), name = 'Diagnostic Session')
		if maxBaudRate > 9600:
			print "Trying to raise baud rate to 19200"
			try:
				self.sendExpectingResponse('\x87\x01\x01\x02','\xC7\x01',"Request Byte rate 19200")
			except communicationError:
				print "Could not raise baudrate"
			else:
				self.sendData('\x87\x02\x03')
				self.conn.baudrate = 19200
		
		self.sendExpectingResponse('\0\0\0\0\xFF\xFF\xFF\xFF', '\0\xFF', name = 'Request Upload')

	def close(self):
		self.sendExpectingResponse(chr(0x35), chr(0x77), name = 'Transfer Exit')
		self.sendExpectingResponse(chr(0x82), chr(0xC2), name = 'Stop Communication')
		self.conn.close()
		
	def _getChecksum(self, data):
		sum = 0
		for byte in data:
			sum += ord(data)
		return sum % 256
	def getBlock(self, TREP, parameter = ''):
		self.sendData(chr(0x36) + chr(TREP) + parameter)
		payload = ''
		counter = 1
		failcounter = 0
		while():
			try:
				response = singleResponse()
			except wrongChecksum, malformedHeader:
				failcounter += 1
				if failcounter == 3:
					raise communicationError("Failed to get the same 265-byte-chunk three times, aborting")
				self.sendData(chr(0x83) + chr(0x36) + pack(">H",counter))
				
				
			else:
				failcounter = 0
				payload += response[4:]
				if len(response) < 255:
					break
				counter += 1
				self.sendData(chr(0x83) + chr(0x36) + pack(">H",counter))
				
		if counter == 1:
			errors = {0x10 : 'General reject',
			0x11: 'Service not supported',
			0x12: 'Sub function not supported',
			0x13: 'Incorrect Message Length',
			0x22: 'Conditions not correct or Request sequence error',
			0x31: 'Request out of range',
			0x50: 'Upload not accepted',
			0x78: 'Response pending',
			0xFA: 'Data not available'}
			if len(response) == 3 and ord(response[0]) == 0x7F:
				errcode = ord(response[2])
				if errcode in errors:
					raise communicationError("VU sent error code: " + errors[errcode])
				else:
					raise communicationError("VU sent unknown error code, message was " + response)
		return payload

	def sendExpectingResponse(self, senddata, expectdata, name = ''):
		print "Sending 'Request %s'" % name
		self.sendData(senddata)
		response = self.singleResponse()
		if response == expectdata:
			print "Got the expected response"
		else:
			raise communicationError("Expected " + expectdata + " as response to " + name + ", got " + response)

	def singleResponse(self):
		header = self.conn.read(4)
		if header[0:3] != '\x80\xF0\xEE':
			raise malformedHeader(header)
		length = ord(header[3])
		dataWithChecksum = self.conn.read(length + 1)
		payload = dataWithChecksum[:-1]
		if _getChecksum(header + payload) == ord(dataWithChecksum[-1]):
			return payload
		else:
			raise wrongChecksum

	def sendData(self, data):
		fullmsg = chr(0x80) + chr(0xEE) + chr(0xF0) + chr(len(data)) + data
		sum = _getChecksum(data)
		self.conn.write(fullmsg + chr(sum))


print "This is completely untested, without some hacking this will not work, and even if it does, it won't do what you expect"

vu = vuSerial()
data = vu.getBlock(1)
vu.close()

