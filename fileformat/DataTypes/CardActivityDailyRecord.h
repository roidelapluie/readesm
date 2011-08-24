#ifndef CARDACTIVITYDAILYRECORD_H
#define CARDACTIVITYDAILYRECORD_H

#include "ActivityChangeInfo.h"
#include "RawCardActivityDailyRecord.h"
#include "Subblocks.h"

class CardActivityDailyRecord : public RawCardActivityDailyRecord {
public:
	Subblocks<ActivityChangeInfo> activityChangeInfos;
	
	CardActivityDailyRecord(const constDataPointer& start) : RawCardActivityDailyRecord(start),
		activityChangeInfos(start + RawCardActivityDailyRecord::staticSize, (activityRecordLength - RawCardActivityDailyRecord::staticSize) / ActivityChangeInfo::staticSize)
	{
		qDebug() << "gen'd CardActivityDailyRecord";
	}
	int size() const {
		if(activityRecordLength < RawCardActivityDailyRecord::staticSize){
			qDebug() << "Size to small in CardActivityDailyRecord";
			return RawCardActivityDailyRecord::staticSize;
		}
		return activityRecordLength;
	}
	void printOn(reporter& o) const {
		RawCardActivityDailyRecord::printOn(o);
		activityChangeInfos.printOn(o);
	}
};

#endif
