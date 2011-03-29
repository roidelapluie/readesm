#ifndef VEHICLEREGISTRATION_H
#define VEHICLEREGISTRATION_H

#include "dataType.h"

struct vehicleRegistration {
	Q_DECLARE_TR_FUNCTIONS(vehicleRegistration)
public:
	int Nation;
	QString Number;
	vehicleRegistration(constDataPointer start) :
		Nation(start[0]), 
		Number(fixedString(start + 1, 14)) 
	{
	}
	friend reporter& operator<<(reporter& report, vehicleRegistration sub) {
		if(report.verbose) report(tr("vehicleRegistrationNation"), formatStrings::nationNumeric(sub.Nation));
		if(sub.Number[0] != 0) report(tr("vehicleRegistrationNumber"), sub.Number);
		return report;
	}
	static bool defval(constDataPointer start) {
		return checkZeros(start, 2) && checkSpaces(start + 2, 13);
	}
};

#endif