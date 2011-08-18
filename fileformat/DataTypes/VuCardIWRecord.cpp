#include "VuCardIWRecord.h"

VuCardIWRecord::VuCardIWRecord(const constDataPointer& filewalker) : RawVuCardIWRecord(filewalker)
{
	FullCardNumber::mapCardNumberToName()[cardNumber.cardNumber] = cardHolderName.toString();
}
