#ifndef TIMESPAN_H
#define TIMESPAN_H TIMESPAN_H

#include "TimeReal.h"

#include <QtCore/QCoreApplication>

class Timespan {
	Q_DECLARE_TR_FUNCTIONS(Timespan)
	public:
	TimeReal begin;
	TimeReal end;

	Timespan(const DataPointer& start);

	QString toString() const;
};

#endif
