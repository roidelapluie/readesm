#include "RawData.h"

RawData::RawData(const DataPointer& start_, int length) :
	DataType(start_),
	DataPointer(start_),
	length_(length)
{}

RawData::RawData(const QByteArray& data_) :
	DataType(DataPointer(data_)),
	DataPointer(data_),
	length_(data_.size())
{}

QString RawData::toString() const{
	QString rv;
	for(int j = 0; j < size(); ++j) rv.append(QString("%1 ").arg(start[j], 2, 16, QChar('0')));
	return rv;
}

void RawData::printOn(Reporter& report) const{
	report.tagValuePair("Raw data(hex encoded)", toString());
}

int RawData::size() const {
	return length_;
}

QByteArray RawData::toQByteArray() const {
	return start.toQByteArray().mid(start.offset, size());
}
