#include "TimeReal.h"
#include <QDebug>
TimeReal::TimeReal(const constDataPointer& start) : QDateTime(QDateTime::fromTime_t(readBigEndianInt4(start)))
{
	((QDateTime*)this)->operator=(this->toUTC());
}

QString TimeReal::toString() const{
	QTime t = time();
	if(t.hour() == 0 && t.minute() == 0 && t.second() == 0){
		return date().toString();
	}
	return QDateTime::toString();
}
