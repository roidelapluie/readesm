#include "CardSlots.h"

CardSlots::CardSlots(const DataPointer& filewalker) : RawCardSlots(filewalker)
{}

void CardSlots::printOn(Reporter& report) const {
	if(cardNumberDriverSlotBegin != cardNumberDriverSlotEnd) {
		if(!cardNumberDriverSlotBegin.isDefaultValue()) 
			report.tagValuePair(tr("cardNumberDriverSlotBegin"), cardNumberDriverSlotBegin.toString());
		if(!cardNumberDriverSlotEnd.isDefaultValue()) 
			report.tagValuePair(tr("cardNumberDriverSlotEnd"), cardNumberDriverSlotEnd.toString());
	} else if(!cardNumberDriverSlotBegin.isDefaultValue()) report.tagValuePair(tr("cardNumberDriverSlot"), cardNumberDriverSlotBegin.toString());

	if(cardNumberCoDriverSlotBegin != cardNumberCoDriverSlotEnd) {
		if(!cardNumberCoDriverSlotBegin.isDefaultValue()) 
			report.tagValuePair(tr("cardNumberCoDriverSlotBegin"), cardNumberCoDriverSlotBegin.toString());
		if(!cardNumberCoDriverSlotEnd.isDefaultValue()) 
			report.tagValuePair(tr("cardNumberCoDriverSlotEnd"), cardNumberCoDriverSlotEnd.toString());
	} else if(!cardNumberCoDriverSlotBegin.isDefaultValue()) report.tagValuePair(tr("cardNumberCoDriverSlot"), cardNumberCoDriverSlotBegin.toString());
}
