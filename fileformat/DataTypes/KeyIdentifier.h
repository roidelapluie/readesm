#ifndef KEYIDENTIFIER_H
#define KEYIDENTIFIER_H


#include "CertificateAuthority.h"
#include "RawKeyIdentifier.h"

class KeyIdentifier : public RawKeyIdentifier {
	Q_DECLARE_TR_FUNCTIONS(KeyIdentifier)
public:
	KeyIdentifier(const constDataPointer& filewalker);
	void printOn(reporter& o) const;
	CertificateAuthority asCertificateAuthority;
};


#endif
