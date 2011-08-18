#ifndef FULLCARDNUMBER_H
#define FULLCARDNUMBER_H


#include "RawFullCardNumber.h"
#include <QMap>

class FullCardNumber : public RawFullCardNumber {
	Q_DECLARE_TR_FUNCTIONS(FullCardNumber)
public:
	FullCardNumber(constDataPointer filewalker);
	QString toString() const;
	virtual void printOn(reporter& o) const;
	static QMap<QString, QString>& mapCardNumberToName();
	
};


#endif
