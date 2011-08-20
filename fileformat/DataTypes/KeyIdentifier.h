#ifndef KEYIDENTIFIER_H
#define KEYIDENTIFIER_H


#include "RawKeyIdentifier.h"
#include "CertificateAuthority.h"

class KeyIdentifier : public RawKeyIdentifier {
	Q_DECLARE_TR_FUNCTIONS(KeyIdentifier)
public:
	KeyIdentifier(const constDataPointer& filewalker);
	void printOn(reporter& o) const;
	CertificateAuthority asCertificateAuthority;
};


#endif
