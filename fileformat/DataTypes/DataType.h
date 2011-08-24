#ifndef DATATYPE_H
#define DATATYPE_H

#include "../constDataPointer.h"
#include "../readTypes.h"
#include "../reporter/reporter.h"


class DataType {
public:
	constDataPointer start;
	DataType(const constDataPointer& nstart);

	friend reporter& operator<<(reporter& report, const DataType& d);
	virtual void printOn(reporter& report) const  = 0;
	virtual int size() const = 0;
	virtual bool operator==(const DataType& other) const;
	virtual bool operator!=(const DataType& other) const;
	virtual bool isDefaultValue() const;
};

#endif
