#ifndef TIMEREAL_H
#define TIMEREAL_H

#include "../constDataPointer.h"
#include "../readTypes.h"

#include <QtCore/QDateTime>

class TimeReal : public QDateTime{
	public:
	TimeReal(const constDataPointer& start);
	QString toString() const;
};

#endif
