#!/usr/bin/python
# -*- coding: utf-8 -*-

"""Handle timestamps in the tachograph data files

The tachograph data files contain timestamps of the beginnings of days at utc."""

import calendar
import time
from struct import pack, unpack

def formatTimestamp(seconds):
	"""Format timestamp as iso date"""
	t=time.gmtime(seconds)
	if(t.tm_hour == 0 and t.tm_min == 0 and t.tm_sec == 0):
		return time.strftime("%Y-%m-%d", t)
	else:
		return time.strftime("%Y-%m-%d %H.%M (%Z)", t)

def dateToTimestamp(datestring):
	"""convert iso date to timestring"""
	t = time.strptime(datestring + " UTC", "%Y-%m-%d %Z")
	return int(calendar.timegm(t))

def packTimestamp(seconds):
	"""pack timestamp into 32-bit big endian bytes"""
	datastring = pack('>I',seconds)
	return datastring

def unpackTimestamp(datastring):
	"""unpack timestamp from 32-bit big endian bytes"""
	return unpack('>I',datastring)[0]
