#include "CardDriverActivity.h"
#include <QtCore/QDebug>

QByteArray CardDriverActivity::readCyclicData(){
	QByteArray rv;
	qDebug() << "oldest" << oldestRecord << "newest" << newestRecord;
	if(newestRecord < oldestRecord) {
		qDebug() << "card full" << rv.size() << size() << cyclicData.size();
		//just copy the circular stuff into a new vector, avoids those boundary problems
		rv.append(cyclicData.toPointer(oldestRecord), cyclicData.size() - oldestRecord);
			qDebug() << "older" << rv.size() << (uint)rv[0] << (uint)rv[rv.size() - 1];
		rv.append(cyclicData.toPointer(), newestRecord);
			qDebug() << "older" << rv.size() << (uint)rv[0] << (uint)rv[rv.size() - 1];
	} else {
		qDebug() << "card not full" << rv.size();
		rv.append(cyclicData.toPointer(oldestRecord), newestRecord - oldestRecord);
		qDebug() << "card not full" << rv.size();
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
	cardActivityDailyRecords.printOn(o);
}
