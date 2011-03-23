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
 
(options, args) = parser.parse_args()

class vuSerial:
	def __init__(self):
		self.conn = Serial(options.serial, 9600, timeout=1)
	def _getChecksum(self, data):
		sum = 0
		for byte in data:
			sum += ord(data)
		return sum % 256
	def _singleResponse(self):
		header = self.conn.read(4)
		length = ord(header[3])
		payload = self.conn.read(length)
		return payload
	def sendData(self, data = "", format = 0x80):
		fullmsg = chr(format) + chr(0xEE) + chr(0xF0) + chr(len(data)) + data
		sum = _getChecksum(data)
		self.conn.write(fullmsg + chr(sum))

this = vuSerial()
print "This does nothing (yet)"