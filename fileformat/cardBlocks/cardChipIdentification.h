#ifndef CARDCHIPIDENTIFICATION_H
#define CARDCHIPIDENTIFICATION_H

#include "cardBlock.h"

#include <QtCore/QString>

///ef ic according to en 726-3
class cardChipIdentification : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(cardChipIdentification)
	public:
	static const int Type = 0x0005;

	QString hexdata;

	virtual QString name() const {
		return tr("Card chip identification");
	}

	cardChipIdentification(constDataPointer filewalker) :
		cardBlock(filewalker),
		hexdata(hexchunk(start + 5, 8))
	{
	}

	virtual void printOn(reporter& o) const {
		o("hex data", hexdata);
	}
};

#endif
