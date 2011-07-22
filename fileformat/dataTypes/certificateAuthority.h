#ifndef CERTIFICATEAUTHORITY_H
#define CERTIFICATEAUTHORITY_H

#include "dataType.h"

class certificateAuthority : public dataType {
	Q_DECLARE_TR_FUNCTIONS(CAid)
	public:
	int numNation;
	QString alphaNation;
	int serialNumber;
	int additionalCoding;
	int identifier;
	bool operator==(const certificateAuthority& other) const {
		return numNation == other.numNation 
			&& alphaNation == other.alphaNation
			&& serialNumber == other.serialNumber 
			&& additionalCoding == other.additionalCoding 
			&& identifier == other.identifier;
	}
	bool operator!=(const certificateAuthority& other) const {
		return !operator==(other);
	}
	certificateAuthority(constDataPointer start);
	void printOn(reporter& report) const;
};

#endif