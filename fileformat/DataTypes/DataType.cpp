#include "DataType.h"
#include <QtCore/QDebug>


DataType::DataType(const DataPointer& nstart) : Block(nstart)
{}

Reporter& operator<<(Reporter& report, const DataType& d){
	d.printOn(report);
	return report;
}

bool DataType::operator==(const DataType& other) const{
	for(int j = 0; j < size(); ++j) if(start[j] != other.start[j]) return false;
	return true;
}

bool DataType::operator!=(const DataType& other) const{
	return !operator==(other);
}

bool DataType::isDefaultValue() const{
	for(int j = 0; j < size(); ++j) if(start[j] != 0x0 && start[j] != 0xFF && start[j] != 0x20) return false;
	return true;
}
