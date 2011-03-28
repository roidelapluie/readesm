#ifndef FULLCARDNUMBER_H
#define FULLCARDNUMBER_H

#include <QString>
#include <QCoreApplication>

#include "dataType.h"

struct fullCardNumber : public dataType {
	Q_DECLARE_TR_FUNCTIONS(fullCardNumber)
public:
	int Type;
	int Nation;
	QString Number;
	fullCardNumber(constDataPointer start);
	void printOn(reporter& report);
	static bool defval(constDataPointer start);
	bool defval() const;
	QString str() const;
	bool operator!=(const fullCardNumber& other) const;
};

#endif