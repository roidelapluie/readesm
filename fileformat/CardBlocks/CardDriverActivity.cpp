#include "CardDriverActivity.h"

CardDriverActivity::CardDriverActivity(const constDataPointer& start)
	: RawCardDriverActivity(start) 
{
	if(newestRecord < oldestRecord) {
		//just copy the circular stuff into a new vector, avoids those boundary problems
		activityDataUncycled.append(cyclicData.toPointer(oldestRecord), cyclicData.length() - oldestRecord);
		activityDataUncycled.append(cyclicData.toPointer(), newestRecord);
	} else {
		activityDataUncycled.append(cyclicData.toPointer(oldestRecord), newestRecord - oldestRecord);
	}
	constDataPointer walker(activityDataUncycled);
	//cardActivityDailyRecords = Subblocks<CardActivityDailyRecord>(walker, activityDataUncycled.size());
	while(walker.bytesLeft()) {
		qDebug() << walker.bytesLeft() <<" Bytes left";
		CardActivityDailyRecord rec(walker);
		cardActivityDailyRecords.push_back(rec);
		walker += rec.size();
		qDebug() << rec.size() << "in next block";
	}
// 		int thissize = BEInt16(walker + 2);
// 		//qDebug() << walker.bytesLeft() <<" Bytes left, " << thissize << "in next block";
// 		if(!thissize) {
// 			qDebug() << "Size 0 for increment. Aborting.";
// 			break;
// 		}
// 		dailyActivityCard d(walker, (thissize - 12) / 2);
// 		acts.push_back(d);
// 		walker += thissize;
// 	}
// 	fine = checkTimes(acts);
// 	qDebug() << "driverActivityData end";

}

void CardDriverActivity::printOn(reporter& o) const{
	//RawCardDriverActivity::printOn(o);
	cardActivityDailyRecords.printOn(o);
}
