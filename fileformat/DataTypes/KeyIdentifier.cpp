#include "KeyIdentifier.h"


KeyIdentifier::KeyIdentifier(const DataPointer& filewalker) : RawKeyIdentifier(filewalker),
	asCertificateAuthority(start)
{}

void KeyIdentifier::printOn(Reporter& o) const {
	if(manufacturerCode == 1) asCertificateAuthority.printOn(o);
	else RawKeyIdentifier::printOn(o);
}
