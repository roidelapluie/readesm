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

#ifndef HELPER_H
#define HELPER_H HELPER_H

#include <QString>
#include <QTextStream>

template <typename T1, typename T2>
void copy(T1 from, T2 to, int number) {
	for(int j = 0; j < number; ++j)
		to[j] = from[j];
}

template <typename T1, typename T2>
bool compare(const T1& from, const T2& to, int number) {
	for(int j = 0; j < number; ++j)
		if(to[j] != from[j]) return false;
	return true;
}

template <typename T>
QString hex(const T& toHex, int w = 0) {
	return QString("%1").arg(toHex, w, 16, QChar('0'));
}
	

template <typename T>
void hexout(QTextStream& o, T ray, int len) {
	for(int j = 0; j < len; ++j) {
		o << hex(int(ray[j]), 2) << " ";
	}
}

template <typename T>
QString stringify(T toStr) {
	QString rv;
	QTextStream s(&rv);
	s << toStr;
	return rv;
}

// template <typename T>
// bool checkchar(const T& i, int length, unsigned char tocheck) {
// 	for(T j = i; j < i + length; ++j)
// 		if(*j != tocheck) return false;
// 	return true;
// }

template <typename T>
bool checkchar(const T& i, int length, unsigned char tocheck) {
	for(int j = 0; j < length; ++j)
		if(i[j] != tocheck) return false;
	return true;
}

template <typename T>
bool checkZeros(const T& i, int length) {
	return checkchar(i, length, 0);
}

template <typename T>
bool checkSpaces(const T& i, int length) {
	return checkchar(i, length, 0x20);
}

#endif
