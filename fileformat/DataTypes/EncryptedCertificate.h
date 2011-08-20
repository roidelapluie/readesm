#ifndef ENCRYPTEDCERTIFICATE_H
#define ENCRYPTEDCERTIFICATE_H

#include "RawEncryptedCertificate.h"
#include "PlainCertificate.h"
#include "RsaPublicKey.h"
#include "Signature.h"
#include "DecryptedCertificate.h"

class EncryptedCertificate : public RawEncryptedCertificate {
	Q_DECLARE_TR_FUNCTIONS(EncryptedCertificate)
protected:
	bool attemptVerification(const RsaPublicKey& key);
	QSharedPointer<DecryptedCertificate> decryptedCertificate;
public:
	bool attemptVerificationFrom(const PlainCertificate& certificate);
	bool attemptVerificationFrom(const EncryptedCertificate& certificate);
	EncryptedCertificate(constDataPointer filewalker);
	virtual void printOn(reporter& report) const;
	bool isVerified() const;
};


#endif
