#include "CardDriverActivity.h"
#include <QtCore/QDebug>

QByteArray CardDriverActivity::readCyclicData(){
	QByteArray rv;
	if(newestRecord < oldestRecord) {
		//just copy the circular stuff into a new vector, avoids those boundary problems
		rv.append(cyclicData.toPointer(oldestRecord), cyclicData.size() - oldestRecord);
		rv.append(cyclicData.toPointer(), newestRecord);
	} else {
		rv.append(cyclicData.toPointer(oldestRecord), newestRecord - oldestRecord);
	}
	return rv;
}
CardDriverActivity::CardDriverActivity(const DataPointer& start) : RawCardDriverActivity(start),
	activityDataUncycled(readCyclicData()),
	cardActivityDailyRecords(Subblocks<CardActivityDailyRecord>::fromTypeAndLength(DataPointer(activityDataUncycled), activityDataUncycled.size()))
{
}

void CardDriverActivity::printOn(Reporter& o) const{
	//RawCardDriverActivity::printOn(o);
	o.writeArray(cardActivityDailyRecords, tr("cardActivityDailyRecords"));
}
