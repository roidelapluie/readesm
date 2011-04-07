#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H

class identification : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(identification)
	public:
	int cardIssuingMemberState;
	QString cardNumber;
	QString cardIssuingAuthorityName;
	Time cardIssueDate;
	Time cardValidityBegin;
	Time cardExpiryDate;
	QString holderSurname;
	QString holderFirstname;
	QString cardHolderBirthDate;
	QString cardHolderPreferredLanguage;

	static const int Type = 0x0520;
	QString name() const {
		return tr("Card Identification");
	}

	identification(constDataPointer filewalker) :
		cardBlock(filewalker), 
		cardIssuingMemberState(start[5]), 
		cardNumber(fixedString(start + 6, 16)), 
		cardIssuingAuthorityName(codepageStringCombination(start + 22, 36)),
		cardIssueDate(BEInt32(start + 58)),
		cardValidityBegin(BEInt32(start + 62)), 
		cardExpiryDate(BEInt32(start + 66)),
		holderSurname(codepageStringCombination(start + 70, 36)),
		holderFirstname(codepageStringCombination(start + 106, 36)), 
		cardHolderBirthDate(BCDDate(start + 142)), 
		cardHolderPreferredLanguage(fixedString(start + 146, 2)) 
	{
	}

	virtual void printOn(reporter& o) const {
		o(tr("cardIssuingMemberState"), formatStrings::nationNumeric(cardIssuingMemberState));
		o(tr("cardNumber"), cardNumber);
		o(tr("cardIssuingAuthorityName"), cardIssuingAuthorityName);
		o(tr("cardIssueDate"), cardIssueDate.str());
		o(tr("cardValidityBegin"), cardValidityBegin.str());
		o(tr("cardExpiryDate"), cardExpiryDate.str());
		o(tr("holderSurname"), holderSurname);
		o(tr("holderFirstNames"), holderFirstname);
		o(tr("cardHolderBirthDate"), cardHolderBirthDate);
		o(tr("cardHolderPreferredLanguage"), cardHolderPreferredLanguage);
	}

	virtual void reportstuff(esmfilehead& esm) {
		esm.title = holderFirstname + " " + holderSurname;
	}
};

#endif
