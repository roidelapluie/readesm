#ifndef PLACERECORD_H
#define PLACERECORD_H

#include "dataType.h"

struct placeRecord {
	Q_DECLARE_TR_FUNCTIONS(placeRecord)
public:
	Time entryTime;
	int entryTypeDailyWorkPeriod;
	int dailyWorkPeriodCountry;
	int dailyWorkPeriodRegion;
	int vehicleOdometerValue;

	placeRecord(constDataPointer start) :
		entryTime(BEInt32(start)), 
		entryTypeDailyWorkPeriod(start[4]),
		dailyWorkPeriodCountry(start[5]), 
		dailyWorkPeriodRegion(start[6]), 
		vehicleOdometerValue(BEInt24(start + 7))
	{
	}

	friend reporter& operator<<(reporter& report, placeRecord sub) {
		report(tr("entryTime"), sub.entryTime.str());
		report(tr("entryTypeDailyWorkPeriod"), formatStrings::dailyWorkPeriod(
				sub.entryTypeDailyWorkPeriod));
		report(tr("dailyWorkPeriodCountry"), formatStrings::nationNumeric(sub.dailyWorkPeriodCountry));
		if(sub.dailyWorkPeriodRegion) 
			report(tr("dailyWorkPeriodRegion"), formatStrings::regionNumeric(sub.dailyWorkPeriodRegion));
		report(tr("vehicleOdometerValue"), sub.vehicleOdometerValue);
		return report;
	}
	static bool defval(constDataPointer start) {
		return checkZeros(start, 10);
	}
};

#endif
