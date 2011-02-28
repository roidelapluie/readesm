#include "myTime.h"

Duration operator-(const Time& a, const Time& b) {
	return Duration(a.timestamp - b.timestamp);
}

QString formatRange(const Time& begin, const Time& end) {
	Duration d = end - begin;
	if(d.length < 86400) return QObject::tr("from") + " " + begin.str() + " " + QObject::tr(
			"on for") + " " + d.str();
	return QObject::tr("from") + " " + begin.str() + " " + QObject::tr("to") + " " + end.str()
			+ " (" + d.str() + ")";
}

QString formatMinutes(int minutes) {
	QString rv;
	if(minutes >= 3* 24* 60 ) {
		rv =  QObject::tr("%1 days ").arg(minutes / (24*60));
		minutes %= 24*60;
	}
	rv += QString("%1:%2")
		.arg(minutes / 60, 2, 10, QChar('0'))
		.arg(minutes % 60, 2, 10, QChar('0'));
	return rv;
}
