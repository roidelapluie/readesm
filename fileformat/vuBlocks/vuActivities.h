#ifndef VUACTIVITIES_H
#define VUACTIVITIES_H VUACTIVITIES_H
#include <QString>
#include <vector>
#include "reporter.h"
#include "vuBlock.h"
#include "Activity.h"
#include "formatStrings.h"
#include "constDataPointer.h"

///See page 161 of l207.pdf
class vuActivities : public vuBlock {
	Q_DECLARE_TR_FUNCTIONS(vuActivities)
	struct CardInOut {
		QString Name;
		Time InTime, OutTime;
		int OdometerIn, OdometerOut;
	};

	dailyActivity DayAct;
	int driven;
	std::vector<CardInOut> CardInOuts;
	Time day;
	Activity readActivity(int start) const;
	Activity readActivity() const;
	
	public:
	virtual QString name() const;
	static const int TREP = 0x2;
	vuActivities(constDataPointer nstart);
	int size() const;
	void CompleteReport(reporter& report) const;
	void BriefReport(reporter& report) const;
	virtual void reportstuff(esmfilehead& esm);
};

#endif
