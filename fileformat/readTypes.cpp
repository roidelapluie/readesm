#include "readTypes.h"

#include <QtCore/QDebug>

QString codepageStringCombination(const constDataPointer& start, int length){
	if(start[0] == 0) return "";
	QString rv = QString::fromLatin1(start.toPointer(1), length - 1).trimmed();
	if(start[0] > 16){
		qDebug() << rv << "does not seem to be a codepage-string-combination";
	}
	return rv;
}

QString fixedString(const constDataPointer& start, int length) {
	QString rv = QString::fromLatin1(start.toPointer(0),length).trimmed();
	if(start[0] <= 16 && start[0] >= 1){
		qDebug() << rv << "might be a codepage-string combination, codepage" << (int)start[0] << "parsing as such";
		return codepageStringCombination(start, length);
	}
	if(start[0] < 0x20 || start[0] == 0xFF){
		if(start[0] == 0 && (start[1] == 0 || start[1] == 0xFF)) return "";
		qDebug() << "String starts with " << (int)start[0] << (int)start[1] << (int)start[2] <<", string is " << rv; 
	}
	return rv;
}

int readBigEndianInt1(const constDataPointer& start) {
	return start[0];
}

int readBigEndianInt2(const constDataPointer& start) {
	return (start[0] << 8) + start[1];
}

int readBigEndianInt3(const constDataPointer& start) {
	return (start[0] << 16) + (start[1] << 8) + start[2];
}

int readBigEndianInt4(const constDataPointer& start) {
	return (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3];
}


QString bcdbyte(unsigned char start)
{
	char rv[3];
	rv[0] = '0' + (start >> 4);
	rv[1] = '0' + (start & 0xF);
	rv[2] = 0;
	return rv;
}

