#include "readTypes.h"

#include <QtCore/QDebug>

bool checkString(const DataPointer& start, int length){
	for(int j = 0; j < length; ++j)
		if(start[j] > 0x20 && start[j] != 0xFF && start[j] != '?') return true;
	return false;
}

QString codepageStringCombination(const DataPointer& start, int length){
	if(!checkString(start + 1, length - 1)) return "";
	QString rv = QString::fromLatin1(start.toPointer(1), length - 1).trimmed();
	if(start[0] > 16){
		qDebug() << rv << "does not seem to be a codepage-string-combination";
	}
	return rv;
}

QString fixedString(const DataPointer& start, int length) {
	if(!checkString(start, length)) return "";
	QString rv = QString::fromLatin1(start.toPointer(0),length).trimmed();
	if(start[0] <= 16 && start[0] >= 1){
		qDebug() << rv << "might be a codepage-string combination, codepage" << (int)start[0] << "parsing as such";
		return codepageStringCombination(start, length);
	}
	return rv;
}

int readBigEndianInt1(const DataPointer& start) {
	return start[0];
}

int readBigEndianInt2(const DataPointer& start) {
	return (start[0] << 8) + start[1];
}

int readBigEndianInt3(const DataPointer& start) {
	return (start[0] << 16) + (start[1] << 8) + start[2];
}

int readBigEndianInt4(const DataPointer& start) {
	return (start[0] << 24) + (start[1] << 16) + (start[2] << 8) + start[3];
}

QString bcdbyte(unsigned char start) {
	return hexByte(start);
}

QString hexByte(unsigned char start) {
	return QString("%1").arg(start, 2, 16, QChar('0'));
}
