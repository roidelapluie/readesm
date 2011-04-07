#ifndef DRIVINGLICENSEINFO_H
#define DRIVINGLICENSEINFO_H

#include "cardBlock.h"

class drivingLicenseInfo : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(drivingLicenseInfo)
	public:
	QString drivingLicenseIssuingAuthorithy;
	static const int Type = 0x0521;
	int drivingLicenseIssuingNation;
	QString drivingLicenseNumber;
	QString name() const {
		return tr("Driving License Info");
	}
	drivingLicenseInfo(constDataPointer filewalker) :
		cardBlock(filewalker), 
		drivingLicenseIssuingAuthorithy(codepageStringCombination(start + 5, 36)), 
		drivingLicenseIssuingNation(start[5 + 36]),
		drivingLicenseNumber(fixedString(start + 5 + 36 + 1, 16)) 
	{
	}

	virtual void printOn(reporter& o) const {
		o(tr("drivingLicenceIssuingAuthorithy"), drivingLicenseIssuingAuthorithy);
		o(tr("drivingLicenceIssuingNation"), formatStrings::nationNumeric(drivingLicenseIssuingNation));
		o(tr("drivingLicenceNumber"), drivingLicenseNumber);
	}
};

#endif
