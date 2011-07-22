#ifndef EXTENDEDSERIALNUMBER_H
#define EXTENDEDSERIALNUMBER_H

#include "dataType.h"

class extendedSerialNumber : public dataType {
	Q_DECLARE_TR_FUNCTIONS(extendedSerialNumber)
public:
	unsigned int serialNumber;
	QString date;
	unsigned char type;
	unsigned char manufacturerCode;
	extendedSerialNumber(constDataPointer start); 
	QString str() const;
	static const int size = 8; 
};

#endif
