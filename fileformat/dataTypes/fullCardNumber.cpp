#include "fullCardNumber.h"

#include "../formatStrings.h"
#include "../helper.h"
#include "../readTypes.h"

fullCardNumber::fullCardNumber(constDataPointer start) :
	dataType(start),
	Type(start[0]), 
	Nation(start[1]), 
	Number(fixedString(start + 2, 16)) 
{
}

void fullCardNumber::printOn(reporter& report) const{
	report(tr("cardType"), formatStrings::equipmentType(Type));
	report(tr("cardIssuingMemberState"), formatStrings::nationNumeric(Nation));
	report(tr("cardNumber"), Number);
}

bool fullCardNumber::defval(constDataPointer start) {
	return checkZeros(start, 3) && checkSpaces(start + 3, 16);
}

bool fullCardNumber::defval() const{
	return (Nation == 0 && Type == 0) || (Nation == 0xFF && Type == 0xFF);
}

QString fullCardNumber::str() const {
	return QString("%1 (%2, %3)")
		.arg(Number)
		.arg(formatStrings::nationNumeric(Nation))
		.arg(formatStrings::equipmentType(Type));
}

bool fullCardNumber::operator!=(const fullCardNumber& other) const{
	return Type != other.Type || Nation != other.Nation || Number != other.Number;
}
