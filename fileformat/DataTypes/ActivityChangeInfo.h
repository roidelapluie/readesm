#ifndef ACTIVITYCHANGEINFO_H
#define ACTIVITYCHANGEINFO_H ACTIVITYCHANGEINFO_H

#include "DataType.h"

class ActivityChangeInfo : public DataType {
	Q_DECLARE_TR_FUNCTIONS(activityChangeInfo)
protected:
	QString timespan() const;
	QString extraString() const;
public:
	int s,c,p,a,t;
	int duration;
	int activity;
	bool isSlotState;
	static const int REST = 0;
	static const int AVAILABLE = 1;
	static const int WORK = 2;
	static const int DRIVING = 3;
	static const int UNKNOWN = 4;
	static const int SHORTREST = 5;
	
	static const int staticSize = 2;

	ActivityChangeInfo(const DataPointer& start);
	void setDuration(int newDuration, bool newIsSlotState);
	float heightHint() const;
	
	QString activityName() const;
	QString color() const;
	static QString formatClock(int time);
	QString toString() const;
	QString className() const;
	int size() const;
	void printOn(Reporter & o) const;
	bool isDefaultValue() const;
	QString title() const;
};


#endif
