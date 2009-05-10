/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef READTYPES_H
#define READTYPES_H
#include <string>
#include <sstream>
#include "typedefs.h"
#include "i18n.h"

string fixedString(iter start, int length){
	ostringstream o;
	int lastchar = length - 1; 
	for(; lastchar; --lastchar) if(start[lastchar] > 0x20) break;
	for(int j = 0; j <= lastchar ; ++j) if(start[j] >= 0x20 && start[j] != 0xFF) o << latin1tounicode(start[j]);
	return o.str();
}

int BEInt16(iter start){
	return (start[0] << 8)+ start[1];
}
int BEInt32(iter start){
	return (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3]; 
}
int BEInt24(iter start){
	return (start[0] << 16) + (start[1] << 8) + start[2]; 
}
string bcdbyte(unsigned char start){
	char rv[3];
	rv[0] = '0' + (start >> 4);
	rv[1] = '0' + (start & 0xF);
	rv[2] = 0;
	return rv;
}
string BCDDate(iter start){
	return bcdbyte(start[0]) + bcdbyte(start[1]) + "-" + bcdbyte(start[2]) + "-" + bcdbyte(start[3]);
}

int LEInt32(iter start){
	return (start[3] << 24) + (start[2] << 16) + (start[1] << 8) + start[0]; 
}

#endif
