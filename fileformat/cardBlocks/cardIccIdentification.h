#ifndef CARDICCIDENTIFICATION_H
#define CARDICCIDENTIFICATION_H

#include "cardBlock.h"

#include "../dataTypes/extendedSerialNumber.h"

#include <QtCore/QString>

///ef icc(integrated circuit card) according to en 726-3
class cardIccIdentification : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(cardIccIdentification)
	public:
	static const int Type = 0x0002;

	unsigned char clockStop;
	extendedSerialNumber cardExtendedSerialNumber;
	QString cardApprovalNumber;
	QString cardPersonaliserID;
	QString embedderIcAssemblerId;
	QString icIdentifier;

	virtual QString name() const {
		return tr("Card ICC identification");
	}

	cardIccIdentification(constDataPointer filewalker) :
		cardBlock(filewalker),
		clockStop(start[5]),
		cardExtendedSerialNumber(start + 6),
		cardApprovalNumber(hexchunk(start + 14, 8)),
		cardPersonaliserID(hexchunk(start + 22, 1)),
		embedderIcAssemblerId(hexchunk(start + 23, 5)),
		icIdentifier(hexchunk(start + 28, 2))
	{
	}

	virtual void printOn(reporter& o) const {
		o(tr("clockStop"),clockStop);
		o(tr("cardExtendedSerialNumber"),cardExtendedSerialNumber.str());
		o(tr("cardApprovalNumber"),cardApprovalNumber);
		o(tr("cardPersonaliserID"),cardPersonaliserID);
		o(tr("embedderIcAssemblerId"),embedderIcAssemblerId);
		o(tr("icIdentifier"),icIdentifier);
	}
};

#endif
