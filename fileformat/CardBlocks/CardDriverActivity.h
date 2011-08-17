#ifndef DRIVERACTIVITYDATA_H
#define DRIVERACTIVITYDATA_H

#include "RawCardDriverActivity.h"
#include "../DataTypes/Subblocks.h"
#include "../DataTypes/CardActivityDailyRecord.h"

class CardDriverActivity : public RawCardDriverActivity {
public:
	Subblocks<CardActivityDailyRecord> cardActivityDailyRecords;
	QByteArray activityDataUncycled;

	CardDriverActivity(const constDataPointer& start);
	void printOn(reporter& o) const;
};

#endif
