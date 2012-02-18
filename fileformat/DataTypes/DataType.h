#ifndef DATATYPE_H
#define DATATYPE_H

#include "../Block.h"
#include "../Reporter/Reporter.h"
#include "../readTypes.h"
#include "../DataPointer.h"

class DataType : public Block {
public:
	DataType(const DataPointer& nstart);

	friend Reporter& operator<<(Reporter& report, const DataType& d);
	virtual void printOn(Reporter& report) const  = 0;
	virtual int size() const = 0;
	virtual bool operator==(const DataType& other) const;
	virtual bool operator!=(const DataType& other) const;
	virtual bool isDefaultValue() const;
};

#endif
