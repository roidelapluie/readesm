#ifndef ACTIVITYCHANGEINFO_H
#define ACTIVITYCHANGEINFO_H ACTIVITYCHANGEINFO_H

#include "../reporter/reporter.h"

class ActivityChangeInfo {
	Q_DECLARE_TR_FUNCTIONS(activityChangeInfo)
	public:
	static const int Break = 0;
	static const int Available = 1;
	static const int Work = 2;
	static const int Driving = 3;
	static const int Driver = 0;
	static const int Codriver = 1;
	static const int Crew = 1;
	static const int staticSize = 2;
	static QString formatDurTime(int offset) {
		return QString("%1:%2")
				.arg(offset / 60, 1, 10, QChar('0'))
				.arg(offset % 60, 2, 10, QChar('0'));
	}

	ActivityChangeInfo() :
		slot(0), manning(0), cardin(0), activity(0), time(0), duration(0) {
	}
	ActivityChangeInfo(const constDataPointer& start) {
		slot = (start[0] & (1 << 7)) >> 7;
		manning = (start[0] & (1 << 6)) >> 6;
		cardin = (start[0] & (1 << 5)) >> 5;
		activity = (start[0] & ((1 << 4) | (1 << 3))) >> 3;
		time = int((start[0] & 7) << 8) + start[1];
	}
	QString astr() const {
		if(activity == Break) return tr("break/rest");
		else if(activity == Available) return tr("availability");
		else if(activity == Work) return tr("work");
		else if(activity == Driving) return tr("driving");
		return tr("unknown activity");
	}
	QString tstr(int offset = 0) const {
		return formatDurTime(time + offset);
	}
	QString toString() const {
		QString rv;
		QTextStream o(&rv);
		o << (slot == Codriver ? tr("co-driver") : tr("driver")) << ", ";
		o << (manning == Crew ? tr("crew") : tr("single")) << ", ";
		o << "card " << (cardin ? tr("not inserted") : tr("inserted")) << ", ";
		o << astr() << " " << tstr();
		return rv;
	}
	void printOn(reporter & o) const {
		o.single(toString());
	}
	int slot, manning, cardin, activity, time, duration;
	friend QTextStream& operator<<(QTextStream& o, const ActivityChangeInfo& a) {
		o << a.toString();
		return o;
	}
	friend reporter& operator<<(reporter& o, const ActivityChangeInfo& a) {
		o(a.tstr(), a.toString());
		return o;
	}
};


#endif
