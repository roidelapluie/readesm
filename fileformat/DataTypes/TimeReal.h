#ifndef TIMEREAL_H
#define TIMEREAL_H

#include "../constDataPointer.h"
#include "../readTypes.h"

#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>

class TimeReal : public QDateTime{
	Q_DECLARE_TR_FUNCTIONS(TimeReal)
	bool valid_;
	public:
	TimeReal(const constDataPointer& start);
	QString toString() const;
	bool isValid() const;
};

#endif
