#include "extendedSerialNumber.h"

#include "../formatStrings.h"
#include "../readTypes.h"

extendedSerialNumber::extendedSerialNumber(constDataPointer start) : dataType(start),
	serialNumber(BEInt32(start)),
	date("20" + bcdbyte(start[5]) + "-" + bcdbyte(start[4])),
	type(start[6]),
	manufacturerCode(start[7])
{
}

QString extendedSerialNumber::str() const{
	return QString("%1 (%2) %3, %4")
		.arg(serialNumber)
		.arg(date)
		.arg(formatStrings::equipmentType(type))
		.arg(formatStrings::manufacturerCode(manufacturerCode));
}
