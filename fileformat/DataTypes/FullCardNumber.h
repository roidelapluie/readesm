#ifndef FULLCARDNUMBER_H
#define FULLCARDNUMBER_H

#include "RawFullCardNumber.h"

#include <QtCore/QMap>

class FullCardNumber : public RawFullCardNumber {
	Q_DECLARE_TR_FUNCTIONS(FullCardNumber)
public:
	FullCardNumber(DataPointer filewalker);
	QString toString() const;
	virtual void printOn(Reporter& o) const;
	static QMap<QString, QString>& mapCardNumberToName();

};


#endif
