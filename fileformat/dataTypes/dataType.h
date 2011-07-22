#ifndef DATATYPE_H
#define DATATYPE_H

#include "../constDataPointer.h"
#include "../reporter/reporter.h"

class dataType {
public:
	constDataPointer start;
	dataType(const constDataPointer nstart) : start(nstart)
	{
	}

	friend reporter& operator<<(reporter& report, const dataType& d){
		d.printOn(report);
		return report;
	}
	virtual void printOn(reporter& report) const{
		report("do not call me",1);
	}
	virtual QString str(){
		return "";
	}
};

#endif
