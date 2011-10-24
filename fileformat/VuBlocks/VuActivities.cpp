#include "VuActivities.h"
#include "../PictureGenerators/SvgDayActivity.h"


VuActivities::VuActivities(const DataPointer& filewalker) : RawVuActivities(filewalker)
{
	if(activityChangeInfos.numberOfBlocks() > 0){
		for(int j = 0; j < activityChangeInfos.numberOfBlocks() - 1; ++j){
			if(activityChangeInfos[j + 1].t != 0) activityChangeInfos[j].setDuration(activityChangeInfos[j + 1].t - activityChangeInfos[j].t, true);
			else activityChangeInfos[j].setDuration(60*24 - activityChangeInfos[j].t, true);
		}
		activityChangeInfos[activityChangeInfos.numberOfBlocks() - 1].setDuration(60*24 - activityChangeInfos[activityChangeInfos.numberOfBlocks() - 1].t, true);
	}
}

QString VuActivities::title() const {
	return tr("Activities on %1").arg(timeReal.date().toString());
}

void VuActivities::printOn(Reporter& report) const {
	report.tagValuePair(tr("timeReal"), timeReal.toString());
	report.tagValuePair(tr("odometerValueMidnight"), QString("%1 km").arg(odometerValueMidnight));
	report.writeArray(vuCardIWRecords, tr("vuCardIWRecords"));
	if(report.allowSvg()){
		for(int slot = 0; slot < 2; ++slot){
			SvgDayActivity visualization;
			int count = 0;
			for(int j = 0; j < activityChangeInfos.numberOfBlocks(); ++j){
				if(activityChangeInfos[j].s == slot){
					visualization.add(activityChangeInfos[j].t, activityChangeInfos[j].duration, activityChangeInfos[j].heightHint(), activityChangeInfos[j].color(), activityChangeInfos[j].toString());
					++count;
				}
			}
			if(count > 1) report.tagValuePair(slot == 0 ? tr("Activities Driver") : tr("Activities Codriver"), visualization.toString());
		}
	}

	report.writeArray(activityChangeInfos, tr("activityChangeInfos"),false);
	report.writeArray(vuPlaceDailyWorkPeriodRecords, tr("vuPlaceDailyWorkPeriodRecords"));
	report.writeArray(specificConditionRecords, tr("specificConditionRecords"));
}

