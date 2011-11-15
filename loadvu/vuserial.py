#!/usr/bin/python
# -*- coding: utf-8 -*-

## Tool to read tachograph data from a vehicle unit connected serially 
## Copyright(C) 2011 Andreas Gölzer

#This is free software: you can redistribute it and/or modify it under the
#terms of the GNU General Public License as published by the Free Software
#Foundation, either version 3 of the License, or (at your option) any later
#version.

#It is distributed in the hope that it will be useful, but WITHOUT ANY
#WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

#You should have received a copy of the GNU General Public License along with
#this program.  If not, see <http://www.gnu.org/licenses/>.

from time import sleep
import serial
from serial.serialutil import SerialException
from struct import pack, unpack

def hexchunk(data):
	rv = ""
	for byte in data:
		rv += " " + hex(ord(byte))
	return rv

class malformedHeader(Exception):
	"""Exception for malformed Header"""
	def __init__(self, received):
		self.received = received
	def __str__(self):
		response = hexchunk(self.received)
		if len(self.received) == 0: response = "nothing"
		return "Expected header 0x80 0xF0 0xEE, but instead got " + response + "."

class communicationError(Exception):
	"""Exception for errors in the vu-protocol (checksum wrong 3 times etc.) """
	def __init__(self, description):
		self.description = description
	def __str__(self):
		return description

class wrongChecksum(Exception):
	"""wrong checksum, should be caught and the wrongly transmitted message rerequested"""
	def __str__(self):
		return "Received message had wrong checksum"

class vuErrorMessage(Exception):
	"""Error Messages from the Vehicle unit decoded according to the legislation"""
	def __init__(self, code, requestSID):
		self.code = code
		self.requestSID = requestSID
	def __str__(self):
		errors = {
			0x10 : 'General reject',
			0x11: 'Service not supported',
			0x12: 'Sub function not supported',
			0x13: 'Incorrect Message Length',
			0x22: 'Conditions not correct or Request sequence error',
			0x31: 'Request out of range',
			0x50: 'Upload not accepted',
			0x78: 'Response pending',
			0xFA: 'Data not available'
		}
		rv = "The VU has rejected the request with SID %s and returned an error instead: " % hex(self.requestSID)
		if self.code in errors:
			rv += errors[self.code]
		else:
			rv += "Unknown error code %s" % hex(self.code)
		return rv
	
class vuSerial:
	"""Class for Serial connections to the vehicle unit"""
	def __init__(self, serialPort, maxBaudRate=115200, verbose=1):
		self.open = False
		self.verbose = verbose
		self.conn = serial.Serial(serialPort, 9600, parity=serial.PARITY_EVEN, timeout=6)
		self.sendRawExpectingResponse('\x81\xEE\xF0\x81\xE0', '\xC1\xEA\x8f', 'Start Communication Request')
		self.sendExpectingResponse('\x10\x81', '\x50\x81', 'Diagnostic Session')

		baudRates = {9600:1, 19200:2, 38400:3, 57600:4, 115200:5}
		for baudRate in sorted(baudRates, reverse=True):
			if maxBaudRate < baudRate:
				continue			
			try:
				self.sendExpectingResponse('\x87\x01\x01' + chr(baudRates[baudRate]),'\xC7\x01', "Request Byte rate %i" % baudRate)
			except communicationError as err:
				print err
			else:
				self.sendComposedData('\x87\x02\x03')
				sleep(0.1) #wait until data is sent
				self.conn.setBaudrate(baudRate)
				sleep(0.1) #wait until transition is handled
				break
		self.sendExpectingResponse('\x35\0\0\0\0\0\xFF\xFF\xFF\xFF', '\x75\0\xFF', name = 'Request Upload')
		self.open = True
	
	def __del__(self):
		self.close()
	
	def close(self):
		"""Close connection if it's still open, send required commands"""
		if self.open:
			self.sendExpectingResponse(chr(0x37), chr(0x77), name = 'Transfer Exit')
			self.sendExpectingResponse(chr(0x82), chr(0xC2), name = 'Stop Communication')
			self.conn.close()
			self.open = False
	
	def _getChecksum(self, data):
		"""simple modulo-256-checksum."""
		sum = 0
		for byte in data:
			sum += ord(byte)
		return sum % 256
	
	def getBlock(self, TREP, parameter = ''):
		"""Download a vehicle unit data block with trep and parameter (date for activity blocks)"""
		if self.verbose >= 1:
			TREPstrings = {
				0x1:"Overview",
				0x2:"Activities recorded on ",
				0x3:"Events and faults",
				0x4:"Detailed speed",
				0x5:"Technical data",
				0x6:"Card download"
			}
			explain = "Requesting to download data for TREP %i" % TREP
			if TREP in TREPstrings:
				explain += " (%s)" % TREPstrings[TREP]
			print explain
		self.sendComposedData(chr(0x36) + chr(TREP) + parameter)
		payload = ''
		counter = 1
		failcounter = 0
		while True:
			try:
				response = self.singleResponse()
				sid = response[0]
				if self.verbose >= 2:
					print "SID " + sid + ", TREP " + response[1] + ", submessagecounter " , unpack(">H",response[2:4])[0]
			except wrongChecksum, malformedHeader:
				failcounter += 1
				if failcounter == 3:
					raise communicationError("Failed to get the same 255-byte-chunk three times, aborting")
				self.sendComposedData(chr(0x83) + sid + pack(">H",counter))
			else:
				failcounter = 0
				if len(response) < 255 and counter == 1:
					payload += response[2:]
				else:
					payload += response[4:]
				if len(response) < 255:
					if self.verbose >= 1:
						print "end of block, got %i Byte"  % len(payload)
					break
				counter += 1
				self.sendComposedData(chr(0x83) + sid + pack(">H",counter))
		return response[0:2] + payload

	def sendRawExpectingResponse(self, rawsenddata, expectdata, name = ''):
		"""Send raw data to the VU, expect a response
		The first message to the vu requires message type 0x81, all other messages should
		be sent using sendExpectingResponse. expectdata should be without header and checksum"""
		if name != '' and self.verbose >= 1:
			print "Sending 'Request %s'" % name
		self.sendData(rawsenddata)
		response = self.singleResponse()
		if response == expectdata:
			if self.verbose >= 1:
				print "Got the expected response"
		else:
			raise communicationError("Expected " + hexchunk(expectdata) + " as response to " + name + ", got " + hexchunk(response))
	
	def sendExpectingResponse(self, senddata, expectdata, name = ''):
		"""send data to the vu after adding header, read back data"""
		return self.sendRawExpectingResponse(self.composeMessage(senddata), expectdata, name)
		
	def singleResponse(self):
		"""Read response from the vehicle unit"""
		header = self.conn.read(4)
		if self.verbose >= 3:
			print "got header:" + hexchunk(header)
		if header[0:3] != '\x80\xF0\xEE':
			raise malformedHeader(header)
		length = ord(header[3])
		dataWithChecksum = self.conn.read(length + 1)
		if self.verbose >= 3:
			print "got data:" + hexchunk(dataWithChecksum)
		payload = dataWithChecksum[:-1]
		if self._getChecksum(header + payload) != ord(dataWithChecksum[-1]):
			raise wrongChecksum
		if length == 3 and ord(payload[0])  == 0x7F:
			if ord(payload[2]) == 0x78:
				if self.verbose >= 1:
					print "Got Response pending message, waiting"
				return self.singleResponse()
			raise vuErrorMessage(ord(payload[2]), ord(payload[1]))  
		return payload

	def sendData(self, data):
		"""Send data to the vehicle unit"""
		if self.verbose >= 3:
			print "sending " + hexchunk(data)
		self.conn.write(data)

	def sendComposedData(self, data):
		"""Send data to the vehicle unit after adding header and checksum"""
		self.sendData(self.composeMessage(data))
	
	def composeMessage(self, data):
		"""add header(0x80 for message type, 0xEE for target VU, 0xF0 for source IDE, length of payload) and checksum to the data""" 
		fullmsg = '\x80\xEE\xF0' + chr(len(data)) + data
		sum = self._getChecksum(fullmsg)
		return fullmsg + chr(sum)
