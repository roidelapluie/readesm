#ifndef ACTIVITYCHANGEINFOCARD_H
#define ACTIVITYCHANGEINFOCARD_H

#include "ActivityChangeInfo.h"

class ActivityChangeInfoCard : public ActivityChangeInfo {
public:
	ActivityChangeInfoCard(const DataPointer& start) : ActivityChangeInfo(start){}
	ActivityChangeInfoCard() : ActivityChangeInfo(DataPointer(QByteArray("\0\0"))){}
};
#endif
