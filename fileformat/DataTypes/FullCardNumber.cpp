#include "FullCardNumber.h"

FullCardNumber::FullCardNumber(DataPointer filewalker) : RawFullCardNumber(filewalker)
{}

QString FullCardNumber::toString() const{
	if(mapCardNumberToName().contains(cardNumber)){
		return tr("%1, card holder: %2")
			.arg(RawFullCardNumber::toString())
			.arg(mapCardNumberToName().value(cardNumber));
	}
	if(cardNumber == "") return "";
	return RawFullCardNumber::toString();
}

void FullCardNumber::printOn(Reporter& o) const {
	RawFullCardNumber::printOn(o);
}

QMap<QString, QString>& FullCardNumber::mapCardNumberToName(){
	static QMap<QString, QString>* map = new QMap<QString, QString>();
	return *map;
}
