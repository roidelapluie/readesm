/* Copyright 2009 Andreas Gölzer

 This file is part of readESM.

 readESM is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 readESM is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 readESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef TIME_H
#define TIME_H TIME_H
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <time.h>

/// for now, a simple wrapper around timestamps.
//TODO: check out boosts classes
class Time {
	public:
	Time(int timestamp_ = 0) :
		timestamp(timestamp_) {
	}
	static Time fromBigEndianChars32(const unsigned char* start) {
		return Time((start[0] << 24) + (start[1] << 16) + (start[2] << 8)
				+ start[3]);
	}
	string str(const std::string& format = "%c") const {
		if(timestamp == -1) return "(undefined)";
		char buffer[40];
		time_t t = timestamp;
		tm* timeinfo = gmtime(&t);
		strftime(buffer, 39, format.c_str(), timeinfo);
		return buffer;
	}
	string datestr() const {
		return str("%x");
	}
	friend std::ostream& operator<<(std::ostream& o, const Time& d) {
		return o << d.str();
	}
	bool operator>(const Time& other) const {
		return timestamp > other.timestamp;
	}
	bool operator<(const Time& other) const {
		return timestamp < other.timestamp;
	}
	int timestamp;
};

///a duration between two times
class Duration {
	public:
	///construct a duration from an int containing a duration in seconds
	Duration(int length_ = 0) :
		length(length_) {
	}
	string str() const {
		ostringstream o;
		o << (*this);
		return o.str();
	}
	friend std::ostream& operator<<(std::ostream& o, const Duration& d) {
		int l = d.length;
		if(l < 0) {
			o << "-";
			l = -l;
		}
		if(l > 86400) {
			o << (l / 86400) << " " << tr("days") << " ";
			l %= 86400;
		}
		return o << (l / 3600) << ":" << std::setw(2) << std::setfill('0')
				<< ((l / 60) % 60) << ":" << std::setw(2) << std::setfill('0')
				<< (l % 60);
	}
	int length;
};

Duration operator-(const Time& a, const Time& b) {
	return Duration(a.timestamp - b.timestamp);
}

string formatRange(const Time& begin, const Time& end) {
	Duration d = end - begin;
	if(d.length < 86400) return tr("from") + " " + begin.str() + " " + tr(
			"on for") + " " + d.str();
	return tr("from") + " " + begin.str() + " " + tr("to") + " " + end.str()
			+ " (" + d.str() + ")";
}

string formatMinutes(int minutes) {
	ostringstream o;
	if(minutes >= 3* 24* 60 ) {o << (minutes / (24*60)) << " " << tr("days") << " ";
	minutes %= 24*60;
}
o << std::setw(2) << std::setfill('0') << (minutes / 60) << ":" << std::setw(2) << std::setfill('0') << (minutes % 60);
return o.str();
}

#endif
