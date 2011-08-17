#include "Signature.h"

#include "config.h"

Signature::Signature(const constDataPointer& start) :
	RawData(start, 128),
	checked(false),
	valid(false)
{
}

QString Signature::toString() const {
#ifdef HAVE_CRYPTO
	return "unchecked";
#else
	return tr("Cannot check Signature, no cryptographic libraries available");
#endif
}