#include "TimeReal.h"

#include <QtCore/QDebug>

TimeReal::TimeReal(const constDataPointer& start) : QDateTime(QDateTime::fromTime_t(readBigEndianInt4(start)))
{
	int timestamp = readBigEndianInt4(start);
	if(timestamp == 0 || timestamp == (int)0xFFFFFFFF) valid_= false;
	((QDateTime*)this)->operator=(this->toUTC());
}

QString TimeReal::toString() const {
	if(!isValid()) return tr("undefined");
	if(time().hour() == 0 && time().minute() == 0 && time().second() == 0){
		return date().toString();
	}
	return QDateTime::toString();
}

bool TimeReal::isValid() const {
	return valid_;
}
