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
	bool allzeros = true;
	for(int j = 0; j < size(); ++j){
		if(start[j] != 0){
			allzeros = false;
			break;
		}
	}
	if(allzeros) return tr("All %1 Byte are zeroed.").arg(size());
	QString rv;
	for(int j = 0; j < size(); ++j) rv.append(QString("%1 ").arg(start[j], 2, 16, QChar('0')));
	return rv;
}

void RawData::printOn(Reporter& report) const{
	report.tagValuePair("Raw data(hex encoded)", toString());
}

QString RawData::className() const{
	return "RawData";
}

int RawData::size() const {
	return length_;
}

QByteArray RawData::toQByteArray() const {
	return start.toQByteArray().mid(start.offset, size());
}
