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

#ifndef READTYPES_H
#define READTYPES_H

#include "constDataPointer.h"

#include <QtCore/QDebug>
#include <QtCore/QString>

inline QString codepageStringCombination(constDataPointer start, int length){
	if(start[0] == 0) return "";
	QString rv = QString::fromLatin1(start.toPointer(1), length - 1).trimmed();
	if(start[0] > 16){
		qDebug() << rv << "does not seem to be a codepage-string-combination";
	}
	return rv;
}

inline QString fixedString(constDataPointer start, int length) {
	QString rv = QString::fromLatin1(start.toPointer(0),length).trimmed();
	if(start[0] <= 16 && start[0] >= 1){
		qDebug() << rv << "might be a codepage-string combination, parsing as such";
		return codepageStringCombination(start, length);
	}
	if(start[0] < 0x20 || start[0] == 0xFF){
		if(start[0] == 0 && (start[1] == 0 || start[1] == 0xFF)) return "";
		qDebug() << "String starts with " << (int)start[0] << (int)start[1] << (int)start[2] <<", string is " << rv; 
	}
	return rv;
}

inline  int BEInt16(constDataPointer start) {
	return (start[0] << 8) + start[1];
}
inline int BEInt32(constDataPointer start) {
	return (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3];
}
inline int BEInt24(constDataPointer start) {
	return (start[0] << 16) + (start[1] << 8) + start[2];
}

inline QString bcdbyte(unsigned char start)
{
	char rv[3];
	rv[0] = '0' + (start >> 4);
	rv[1] = '0' + (start & 0xF);
	rv[2] = 0;
	return rv;
}

inline QString BCDDate(constDataPointer start)
{
	return bcdbyte(start[0]) 
		+ bcdbyte(start[1])
		+ "-" + bcdbyte(start[2])
		+ "-" + bcdbyte(start[3]);
}

inline int LEInt32(constDataPointer start) {
	return (start[3] << 24) + (start[2] << 16) + (start[1] << 8) + start[0];
}

#endif
