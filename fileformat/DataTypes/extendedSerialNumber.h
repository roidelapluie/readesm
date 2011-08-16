#ifndef EXTENDEDSERIALNUMBER_H
#define EXTENDEDSERIALNUMBER_H

#include "dataType.h"
#include "../formatStrings.h"

class extendedSerialNumber : public dataType {
	Q_DECLARE_TR_FUNCTIONS(extendedSerialNumber)
public:
	unsigned int serialNumber;
	QString date;
	unsigned char type;
	unsigned char manufacturerCode;
	extendedSerialNumber(constDataPointer start) : dataType(start),
		serialNumber(BEInt32(start)),
		date("20" + bcdbyte(start[5]) + "-" + bcdbyte(start[4])),
		type(start[6]),
		manufacturerCode(start[7])
	{
	}

	QString str() const{
		return QString("%1 (%2) Type %3, %4")
			.arg(serialNumber)
			.arg(date)
			.arg(type)
			.arg(formatStrings::manufacturerCode(manufacturerCode));
	}
	static const int size = 8; 
};

#endif
