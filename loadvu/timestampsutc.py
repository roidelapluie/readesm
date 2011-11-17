#!/usr/bin/python
# -*- coding: utf-8 -*-


import calendar
import time
from struct import pack, unpack

def formatTimestamp(seconds):
	t=time.gmtime(seconds)
	if(t.tm_hour == 0 and t.tm_min == 0 and t.tm_sec == 0):
		return time.strftime("%Y-%m-%d", t)
	else:
		return time.strftime("%Y-%m-%d %H.%M (%Z)", t)

def dateToTimestamp(datestring):
	t = time.strptime(datestring + " UTC", "%Y-%m-%d %Z")
	return int(calendar.timegm(t))

def packTimestamp(seconds):
	datastring = pack('>I',seconds)
	return datastring

def unpackTimestamp(datastring):
	return unpack('>I',datastring)[0]