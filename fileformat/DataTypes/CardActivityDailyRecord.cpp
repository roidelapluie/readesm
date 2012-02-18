#include "CardActivityDailyRecord.h"

#include "../PictureGenerators/SvgDayActivity.h"

#include <QtCore/QDebug>

CardActivityDailyRecord::CardActivityDailyRecord(const DataPointer& start) : RawCardActivityDailyRecord(start),
	activityChangeInfos(Subblocks<ActivityChangeInfo>::fromTypeAndLength(start + RawCardActivityDailyRecord::staticSize, activityRecordLength - RawCardActivityDailyRecord::staticSize))
{
	if(activityChangeInfos.numberOfBlocks() > 0){
		for(int j = 0; j < activityChangeInfos.numberOfBlocks() - 1; ++j){
			if(activityChangeInfos[j + 1].t != 0) activityChangeInfos[j].setDuration(activityChangeInfos[j + 1].t - activityChangeInfos[j].t, false);
			else activityChangeInfos[j].setDuration(60*24 - activityChangeInfos[j].t, false);
		}
		activityChangeInfos[activityChangeInfos.numberOfBlocks() - 1].setDuration(60*24 - activityChangeInfos[activityChangeInfos.numberOfBlocks() - 1].t, false);
	}
}

int CardActivityDailyRecord::size() const {
	if(activityRecordLength < RawCardActivityDailyRecord::staticSize){
		qDebug() << "Size to small in CardActivityDailyRecord";
		return RawCardActivityDailyRecord::staticSize;
	}
	if(activityRecordLength > RawCardActivityDailyRecord::staticSize + 2 * 60 * 24){
		qDebug() << "Size of CardActivityDailyRecord excessive:" << activityRecordLength;
	}
	return activityRecordLength;
}

void CardActivityDailyRecord::printOn(Reporter& o) const {
	RawCardActivityDailyRecord::printOn(o);
	if(o.allowSvg()){
		SvgDayActivity visualization;
		for(int j = 0; j < activityChangeInfos.numberOfBlocks(); ++j){
			visualization.add(activityChangeInfos[j].t, activityChangeInfos[j].duration, activityChangeInfos[j].heightHint(), activityChangeInfos[j].color(), activityChangeInfos[j].toString());
		}
		o.tagValuePair(tr("Visualization"), visualization.toString());
	}

	o.writeArray(activityChangeInfos, tr("activityChangeInfos"), false);
}

QString CardActivityDailyRecord::title() const {
	return tr("Activities on %1").arg(activityRecordDate.toString());
}
