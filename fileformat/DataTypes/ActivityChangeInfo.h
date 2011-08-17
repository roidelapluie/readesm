#ifndef ACTIVITYCHANGEINFO_H
#define ACTIVITYCHANGEINFO_H ACTIVITYCHANGEINFO_H

#include "../reporter/reporter.h"

class ActivityChangeInfo {
	Q_DECLARE_TR_FUNCTIONS(activityChangeInfo)
	public:
	int s,c,p,a,t;
	static const int staticSize = 2;

	ActivityChangeInfo(const constDataPointer& start) :
		s((start[0] & (1 << 7)) >> 7),
		c((start[0] & (1 << 6)) >> 6),
		p((start[0] & (1 << 5)) >> 5),
		a((start[0] & ((1 << 4) | (1 << 3))) >> 3),
		t(int((start[0] & 7) << 8) + start[1])
	{}
	
	QString activityName() const {
		if(a == 0) return tr("break/rest");
		else if(a == 1) return tr("availability");
		else if(a == 2) return tr("work");
		else if(a == 3) return tr("driving");
		return tr("unknown activity");
	}
	QString formatClock(int offset = 0) const {
		return QString("%1:%2")
				.arg(t / 60, 1, 10, QChar('0'))
				.arg(t % 60, 2, 10, QChar('0'));
	}
	QString toString() const {
		QString rv = QString("s%1 c%2 p%3 a%4, ").arg(s).arg(c).arg(p).arg(a) +  QString("%1: ").arg(formatClock());
		if(p == 0){
			rv += tr("Card inserted") + ", ";
			rv += (s == 0 ? tr("driver slot") : tr("co-driver slot")) + ", ";
			rv += (c == 0 ? tr("single") : tr("crew")) + ", ";
			rv += activityName();
		} else {
			rv += tr("Card not inserted or withdrawn, ");
			rv += (s == 0 ? tr("driver slot") : tr("co-driver slot")) + ", ";
			rv += (c == 0 ? tr("following activity unknown") : tr("following activity manually entered")) + ", ";
			rv += activityName();
		}
		return rv;
	}
	void printOn(reporter & o) const {
		o.single(toString());
	}
	friend reporter& operator<<(reporter& o, const ActivityChangeInfo& a) {
		a.printOn(o);
		return o;
	}
};


#endif
