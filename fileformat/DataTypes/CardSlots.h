#ifndef CARDSLOTS_H
#define CARDSLOTS_H

#include "RawCardSlots.h"

class CardSlots : public RawCardSlots {
	Q_DECLARE_TR_FUNCTIONS(CardSlots)
public:
	CardSlots(const constDataPointer& filewalker);
	virtual void printOn(reporter& o) const;
};


#endif
