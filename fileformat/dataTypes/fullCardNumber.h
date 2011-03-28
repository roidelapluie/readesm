#ifndef FULLCARDNUMBER_H
#define FULLCARDNUMBER_H

struct fullCardNumber {
	Q_DECLARE_TR_FUNCTIONS(fullCardNumber)
public:
	int Type;
	int Nation;
	QString Number;
	fullCardNumber(constDataPointer start) :
		Type(start[0]), 
		Nation(start[1]), 
		Number(fixedString(start + 2, 16)) 
	{
	}
	friend reporter& operator<<(reporter& report, fullCardNumber sub) {
		report(tr("cardType"), formatStrings::equipmentType(sub.Type));
		report(tr("cardIssuingMemberState"), formatStrings::nationNumeric(sub.Nation));
		report(tr("cardNumber"), sub.Number);
		return report;
	}
	static bool defval(constDataPointer start) {
		return checkZeros(start, 3) && checkSpaces(start + 3, 16);
	}
	bool defval() const{
		return (Nation == 0 && Type == 0) || (Nation == 0xFF && Type == 0xFF);
	}
	QString str() const {
		return QString("%1 (%2, %3)")
			.arg(Number)
			.arg(formatStrings::nationNumeric(Nation))
			.arg(formatStrings::equipmentType(Type));
	}
	
	bool operator!=(const fullCardNumber& other) const{
		return Type != other.Type || Nation != other.Nation || Number != other.Number;
	}
};

#endif