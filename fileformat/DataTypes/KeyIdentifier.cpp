#include "KeyIdentifier.h"


KeyIdentifier::KeyIdentifier(const constDataPointer& filewalker) : RawKeyIdentifier(filewalker),
	asCertificateAuthority(start)
{}

void KeyIdentifier::printOn(reporter& o) const {
	if(manufacturerCode == 1) asCertificateAuthority.printOn(o);
	else RawKeyIdentifier::printOn(o);
}
