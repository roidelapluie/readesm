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

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTextStream>

#include <time.h>

/// for now, a simple wrapper around timestamps.
//TODO: use qdatetime
class Time {
	public:
	Time(int timestamp_ = 0) :
		timestamp(timestamp_) {
	}
	static Time fromBigEndianChars32(const unsigned char* start) {
		return Time((start[0] << 24) + (start[1] << 16) + (start[2] << 8)
				+ start[3]);
	}
	QString str(const QString& format = "%c") const {
		if(timestamp == -1) return "(undefined)";
		char buffer[40];
		time_t t = timestamp;
		tm* timeinfo = gmtime(&t);
		strftime(buffer, 39, format.toLocal8Bit(), timeinfo);
		return QString::fromLocal8Bit(buffer);
	}
	QString datestr(const QString& format = "%x") const {
		return str(format);
	}
	friend QTextStream& operator<<(QTextStream& o, const Time& d) {
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
	QString str() const {
		QString rv;
		QTextStream o(&rv);
		o << (*this);
		return rv;
	}
	friend QTextStream& operator<<(QTextStream& o, const Duration& d) {
		int l = d.length;
		if(l < 0) {
			o << "-";
			l = -l;
		}
		if(l > 86400) {
			o << (l / 86400) << " " << QObject::tr("days") << " ";
			l %= 86400;
		}
		o << QString("%1:%2:%3")
			.arg(l / 3600, 1, 10, QChar('0'))
			.arg((l / 60) % 60, 2, 10, QChar('0'))
			.arg(l % 60, 2, 10, QChar('0'));
		return o;
	}
	int length;
};

Duration operator-(const Time& a, const Time& b);
QString formatRange(const Time& begin, const Time& end);
QString formatMinutes(int minutes);

#endif
