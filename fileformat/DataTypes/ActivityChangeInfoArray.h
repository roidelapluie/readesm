#ifndef ACTIVITYCHANGEINFOARRAY_H
#define ACTIVITYCHANGEINFOARRAY_H

#include "ActivityChangeInfoCard.h"
#include "../PictureGenerators/SvgDayActivity.h"

#include <QtCore/QDebug>


bool prepareActivityChangeInfoArray(Subblocks<ActivityChangeInfo> activities){
	//set durations
	for(int j = 0; j < activities.size() - 1; ++j){
		if(activities[j + 1].t != 0) activities[j].duration = activities[j + 1].t - activities[j].t;
		else activities[j].duration = 60*24 - activities[j].t;
	}
	activities[activities.size() - 1].duration = 60*24 - activities[activities.size() - 1].t;
	return true;
}

SvgDayActivity visualizeActivityChangeInfoArray(Subblocks<ActivityChangeInfo> activities){
				SvgDayActivity visualization;
			for(int j = 0; j < Activities.size(); ++j){
				visualization.add(Activities[j].t,Activities[j].duration,(Activities[j].a+1)*10,Activities[j].color(),Activities[j].toString());
			}
}

class ActivityChangeInfoArray : public DataType {
	Q_DECLARE_TR_FUNCTIONS(ActivityChangeInfoArray)
private:
	int dataSize;
	bool invalid;
	QVector<ActivityChangeInfoCard> Activities;
public:
	ActivityChangeInfoArray(const DataPointer& filewalker, int dataSize) : DataType(filewalker), dataSize(dataSize) {
		if(start.bytesLeft() < dataSize) invalid = true;
		else {
			invalid = false;
			for(int j = 0; j < dataSize; j += 2){
				Activities.append(ActivityChangeInfoCard(start + j));
			}
			for(int j = 0; j < Activities.size() - 1; ++j){
				if(Activities[j + 1].t != 0) Activities[j].duration = Activities[j + 1].t - Activities[j].t;
				else Activities[j].duration = 60*24 - Activities[j].t;
			}
			Activities[Activities.size() - 1].duration = 60*24 - Activities[Activities.size() - 1].t;
		}
	}
	void printOn(Reporter& o) const {
		if(invalid){
			o.tagValuePair("invalid", "size is larger than remaining space");
		}
		if(o.allowSvg()){
			SvgDayActivity visualization;
			for(int j = 0; j < Activities.size(); ++j){
				visualization.add(Activities[j].t,Activities[j].duration,(Activities[j].a+1)*10,Activities[j].color(),Activities[j].toString());
			}
			o.tagValuePair("Activities", visualization.toString());
		}
		for(int j = 0; j < Activities.size(); ++j) o << Activities[j];
	}
	int size() const {
		return dataSize;
	}
};
#endif
