#include "Timespan.h"

Timespan::Timespan(const constDataPointer& start) :
	begin(start),
	end(start + 4)
{}

QString Timespan::toString() const {
	if(!begin.isValid() && !end.isValid()) return tr("undefined");
	if(begin.isValid() && !end.isValid()) return tr("From %1 on, end undefined").arg(begin.toString());
	if(!begin.isValid() && end.isValid()) return tr("From undefined time until %1").arg(end.toString());
	if(begin == end) return begin.toString();
	int l = begin.secsTo(end);
	QString duration;
	if(l > 86400) {
		duration = tr("%1 days").arg(l / 86400) + " ";
		l %= 86400;
	}
	duration.append(QString("%1:%2:%3")
		.arg(l / 3600, 1, 10, QChar('0'))
		.arg((l / 60) % 60, 2, 10, QChar('0'))
		.arg(l % 60, 2, 10, QChar('0')));

	if(begin.date() == end.date()) {
		return tr("On %1, from %2 to %3 (%4)")
			.arg(begin.date().toString())
			.arg(begin.time().toString())
			.arg(end.time().toString())
			.arg(duration);
	} else {
		return tr("From %1 to %2 (%4)")
			.arg(begin.toString())
			.arg(end.toString())
			.arg(duration);
	}
}
