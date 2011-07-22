#include "certificateAuthority.h"

#include "../formatStrings.h"
#include "../readTypes.h"
#include "../helper.h"

certificateAuthority::certificateAuthority(constDataPointer start) : dataType(start),
	numNation(start[0]), 
	alphaNation(fixedString(start + 1, 3)),
	serialNumber(start[4]), 
	additionalCoding(BEInt16(start + 5)),
	identifier(start[7]) 
{
}

void certificateAuthority::printOn(reporter& o) const{
	o(tr("nationNumeric"), formatStrings::nationNumeric(numNation));
	o(tr("nationAlpha"), alphaNation);
	o(tr("keySerialNumber"), serialNumber);
	o(tr("additionalInfo"), hex(additionalCoding,4));
	o(tr("caIdentifier"), identifier);
}
