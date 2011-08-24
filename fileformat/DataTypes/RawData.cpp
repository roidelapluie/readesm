#include "RawData.h"

RawData::RawData(const constDataPointer& start, int length) :
	constDataPointer(start),
	length_(length)
{}

RawData::RawData(const QByteArray& o) :
	constDataPointer(o),
	length_(o.size())
{}

QString RawData::toString() const{
	QString rv;
	for(int j = 0; j < length(); ++j) rv.append(QString("%1 ").arg(operator[](j), 2, 16, QChar('0')));
	return rv;
}

int RawData::length() const {
	return length_;
}

QByteArray RawData::toQByteArray() const {
	return data.mid(offset, length_);
}
