#ifndef DRIVERACTIVITYDATA_H
#define DRIVERACTIVITYDATA_H

#include "RawCardDriverActivity.h"
#include "../DataTypes/CardActivityDailyRecord.h"
#include "../DataTypes/Subblocks.h"

class CardDriverActivity : public RawCardDriverActivity {
	QByteArray readCyclicData();
public:
	QByteArray activityDataUncycled;
	Subblocks<CardActivityDailyRecord> cardActivityDailyRecords;

	CardDriverActivity(const DataPointer& start);
	void printOn(Reporter& o) const;
};

#endif
