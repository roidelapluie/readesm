#ifndef TIMEREAL_H
#define TIMEREAL_H

#include "../constDataPointer.h"
#include "../readTypes.h"
#include <QDateTime>

class TimeReal : public QDateTime{
	public:
	TimeReal(const constDataPointer& start) : QDateTime(QDateTime::fromTime_t(readBigEndianInt4(start)))
	{
		((QDateTime*)this)->operator=(this->toUTC());
	}
};

#endif
