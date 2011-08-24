#include "CardSlots.h"

CardSlots::CardSlots(const constDataPointer& filewalker) : RawCardSlots(filewalker)
{}

void CardSlots::printOn(reporter& report) const {
	if(cardNumberDriverSlotBegin != cardNumberDriverSlotEnd) {
		if(!cardNumberDriverSlotBegin.isDefaultValue()) 
			report(tr("cardNumberDriverSlotBegin"), cardNumberDriverSlotBegin.toString());
		if(!cardNumberDriverSlotEnd.isDefaultValue()) 
			report(tr("cardNumberDriverSlotEnd"), cardNumberDriverSlotEnd.toString());
	} else if(!cardNumberDriverSlotBegin.isDefaultValue()) report(tr("cardNumberDriverSlot"), cardNumberDriverSlotBegin.toString());

	if(cardNumberCoDriverSlotBegin != cardNumberCoDriverSlotEnd) {
		if(!cardNumberCoDriverSlotBegin.isDefaultValue()) 
			report(tr("cardNumberCoDriverSlotBegin"), cardNumberCoDriverSlotBegin.toString());
		if(!cardNumberCoDriverSlotEnd.isDefaultValue()) 
			report(tr("cardNumberCoDriverSlotEnd"), cardNumberCoDriverSlotEnd.toString());
	} else if(!cardNumberCoDriverSlotBegin.isDefaultValue()) report(tr("cardNumberCoDriverSlot"), cardNumberCoDriverSlotBegin.toString());
}
