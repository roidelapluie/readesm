#include "VuCardIWRecord.h"

VuCardIWRecord::VuCardIWRecord(const DataPointer& filewalker) : RawVuCardIWRecord(filewalker)
{
	FullCardNumber::mapCardNumberToName()[cardNumber.cardNumber] = cardHolderName.toString();
}
