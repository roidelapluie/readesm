#ifndef CARDACTIVITYDAILYRECORD_H
#define CARDACTIVITYDAILYRECORD_H

#include "ActivityChangeInfoArray.h"
#include "RawCardActivityDailyRecord.h"

#include <QtCore/QDebug>

class CardActivityDailyRecord : public RawCardActivityDailyRecord {
public:
	ActivityChangeInfoArray activityChangeInfos;
	
	CardActivityDailyRecord(const DataPointer& start) : RawCardActivityDailyRecord(start),
		activityChangeInfos(start + RawCardActivityDailyRecord::staticSize, activityRecordLength - RawCardActivityDailyRecord::staticSize)
	{
	}
	int size() const {
		if(activityRecordLength < RawCardActivityDailyRecord::staticSize){
			qDebug() << "Size to small in CardActivityDailyRecord";
			return RawCardActivityDailyRecord::staticSize;
		}
		if(activityRecordLength > RawCardActivityDailyRecord::staticSize + 2 * 60 * 24){
			qDebug() << "Size of CardActivityDailyRecord excessive:" <<  activityRecordLength;
		}
		return activityRecordLength;
	}
	void printOn(Reporter& o) const {
		RawCardActivityDailyRecord::printOn(o);
		activityChangeInfos.printOn(o);
	}
};

#endif
