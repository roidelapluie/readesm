#ifndef DATATYPE_H
#define DATATYPE_H

#include "../DataPointer.h"
#include "../readTypes.h"
#include "../Reporter/Reporter.h"
#include "../Block.h"

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
