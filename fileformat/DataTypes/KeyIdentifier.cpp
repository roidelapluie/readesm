#include "KeyIdentifier.h"

#include "CertificateAuthority.h"
#include "RawKeyIdentifier.h"

KeyIdentifier::KeyIdentifier(const DataPointer& filewalker) : DataType(filewalker)
{
	if(start[7] == 1) content = QSharedPointer<DataType>(new CertificateAuthority(start));
	else content = QSharedPointer<DataType>(new RawKeyIdentifier(start));
}

int KeyIdentifier::size() const {
	return 8;
}

QString KeyIdentifier::className() const {
	return "KeyIdentifier";
}

void KeyIdentifier::printOn(Reporter& o) const {
	if(content) content->printOn(o);
}
