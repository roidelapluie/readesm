#ifndef RAWDATA_H
#define RAWDATA_H

#include "DataType.h"
#include "../DataPointer.h"

class RawData : public DataType, public DataPointer {
public:
	int length_;
	RawData(const DataPointer& start, int length);
	RawData(const QByteArray& o);
	QString toString() const;
	QString className() const;
	
	int size() const;
	QByteArray toQByteArray() const;
	void printOn(Reporter& report) const;
};

#endif
