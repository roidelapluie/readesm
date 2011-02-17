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
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

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
string hex(T toHex, int w = 0) {
	ostringstream s;
	s << std::hex << std::setw(w) << std::setfill('0') << toHex;
	return s.str();
}

template <typename T>
void hexout(T ray, int len) {
	for(int j = 0; j < len; ++j) {
		std::cout << hex<int> (int(ray[j])) << " ";
	}
}

template <typename T>
void hexout(std::ostream& o, T ray, int len) {
	for(int j = 0; j < len; ++j) {
		o << hex<int> (int(ray[j])) << " ";
	}
}

template <typename T>
string stringify(T toStr) {
	ostringstream s;
	s << toStr;
	return s.str();
}

template <typename T>
bool checkchar(const T& i, int length, unsigned char tocheck) {
	for(T j = i; j < i + length; ++j)
		if(*j != tocheck) return false;
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
