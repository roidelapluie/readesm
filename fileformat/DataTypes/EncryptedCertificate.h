#ifndef ENCRYPTEDCERTIFICATE_H
#define ENCRYPTEDCERTIFICATE_H

#include "RawEncryptedCertificate.h"
#include "../DataTypes/RsaPublicKey.h"
#include "../DataTypes/RawData.h"
#include "../DataTypes/Signature.h"


class EncryptedCertificate : public RawEncryptedCertificate {
	Q_DECLARE_TR_FUNCTIONS(EncryptedCertificate)
protected:
	bool verified;
	bool attemptVerification(const RsaPublicKey& key);
public:
	EncryptedCertificate(constDataPointer filewalker);
};


#endif
