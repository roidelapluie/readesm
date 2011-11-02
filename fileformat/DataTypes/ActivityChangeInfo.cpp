#include "ActivityChangeInfo.h"

const int ActivityChangeInfo::REST;
const int ActivityChangeInfo::AVAILABLE;
const int ActivityChangeInfo::WORK;
const int ActivityChangeInfo::DRIVING;
const int ActivityChangeInfo::UNKNOWN;
const int ActivityChangeInfo::SHORTREST;

const int ActivityChangeInfo::staticSize;

ActivityChangeInfo::ActivityChangeInfo(const DataPointer& start) : DataType(start),
	s((start[0] & (1 << 7)) >> 7),
	c((start[0] & (1 << 6)) >> 6),
	p((start[0] & (1 << 5)) >> 5),
	a((start[0] & ((1 << 4) | (1 << 3))) >> 3),
	t(int((start[0] & 7) << 8) + start[1]),
	duration(0),
	activity(a)
{}

void ActivityChangeInfo::setDuration(int newDuration, bool newIsSlotState){
	duration = newDuration;
	isSlotState = newIsSlotState;
	if(a == 0 && duration < 15) activity = SHORTREST;
	if(!isSlotState && (p != 0) && (c == 0)) activity = UNKNOWN;
}

float ActivityChangeInfo::heightHint() const {
	switch(activity){
		case REST: return 0.3;
		case AVAILABLE: return 0.15;
		case WORK: return 0.7;
		case DRIVING: return 1.0;
		case UNKNOWN: return 0.2;
		case SHORTREST: return 0.4;
	}
	return 0.5;
}

QString ActivityChangeInfo::activityName() const {
	switch(activity){
		case REST: return tr("break/rest");
		case AVAILABLE: return tr("availability");
		case WORK: return tr("work");
		case DRIVING: return tr("driving");
		case UNKNOWN: return tr("unknown");
		case SHORTREST: return tr("short break");
	}
	return tr("unknown activity");
}
QString ActivityChangeInfo::color() const {
	switch(activity){
		case REST: return "red";
		case AVAILABLE: return "black";
		case WORK: return "yellow";
		case DRIVING: return "green";
		case UNKNOWN: return "purple";
		case SHORTREST: return "orange";
	}
	return "blue";
}

QString ActivityChangeInfo::formatClock(int time){
	return QString("%1:%2")
			.arg(time / 60, 1, 10, QChar('0'))
			.arg(time % 60, 2, 10, QChar('0'));
}
QString ActivityChangeInfo::timespan() const {
	return tr("from %1 to %2 (%3 h)")
		.arg(formatClock(t))
		.arg(formatClock(t+duration))
		.arg(formatClock(duration));
}

QString ActivityChangeInfo::extraString() const {
	QString rv;
	if(isSlotState){
		rv += (p == 0 ? tr("Card inserted") : tr("Card not inserted or withdrawn"))+ ", ";
		rv += (s == 0 ? tr("driver slot") : tr("co-driver slot")) + ", ";
		rv += (c == 0 ? tr("single") : tr("crew"));
	} else {
		if(p == 0){
			rv += tr("Card inserted") + ", ";
			rv += (s == 0 ? tr("driver slot") : tr("co-driver slot")) + ", ";
			rv += (c == 0 ? tr("single") : tr("crew"));
		} else {
			rv += tr("Card not inserted or withdrawn") + ", ";
			rv += (s == 0 ? tr("driver slot") : tr("co-driver slot")) + ", ";
			rv += (c == 0 ? tr("following activity unknown") : tr("following activity manually entered"));
		}
	}
	rv += QString(" (s=%1, c=%2, p=%3, a=%4, t=%5)").arg(s).arg(c).arg(p).arg(a).arg(t);
	return rv;
}

QString ActivityChangeInfo::toString() const {
	return activityName() + ", " + timespan() + ": " + extraString();
}

int ActivityChangeInfo::size() const {
	return 2;
}

void ActivityChangeInfo::printOn(Reporter & o) const {
	o.tagValuePair(activityName() + ", " + timespan(), extraString());
}
bool ActivityChangeInfo::isDefaultValue() const {
	return false;
}

QString ActivityChangeInfo::title() const {
	return tr("%1 for %2 h")
		.arg(activityName())
		.arg(formatClock(duration));
}
