#ifndef CARDACTIVITYDAILYRECORD_H
#define CARDACTIVITYDAILYRECORD_H

#include "Subblocks.h"
#include "ActivityChangeInfo.h"
#include "RawCardActivityDailyRecord.h"


class CardActivityDailyRecord : public RawCardActivityDailyRecord {
public:
	Subblocks<ActivityChangeInfo> activityChangeInfos;
	
	CardActivityDailyRecord(const DataPointer& start);
	int size() const;
	void printOn(Reporter& o) const;
	QString title() const;
};

#endif
