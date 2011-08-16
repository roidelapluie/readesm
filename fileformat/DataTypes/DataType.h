#ifndef DATATYPE_H
#define DATATYPE_H

#include "../constDataPointer.h"
#include "../reporter/reporter.h"
#include "../readTypes.h"

class DataType {
public:
	constDataPointer start;
	DataType(const constDataPointer nstart) : start(nstart)
	{
	}

	friend reporter& operator<<(reporter& report, const DataType& d){
		d.printOn(report);
		return report;
	}
	virtual void printOn(reporter& report) const  = 0;
	virtual QString toString(){
		return "";
	}
	virtual int size() const = 0;
	virtual bool operator==(const DataType& other) const{
		for(int j = 0; j < size(); ++j) if(start[j] != other.start[j]) return false;
		return true;
	}
	virtual bool operator!=(const DataType& other) const{
		return !operator==(other);
	}
};

#endif
