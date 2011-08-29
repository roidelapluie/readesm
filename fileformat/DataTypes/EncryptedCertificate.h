#ifndef ENCRYPTEDCERTIFICATE_H
#define ENCRYPTEDCERTIFICATE_H
#include "RawEncryptedCertificate.h"

#include "config.h"
#ifdef HAVE_CRYPTO

#include "DecryptedCertificate.h"
#include "RsaPublicKey.h"
#include "PlainCertificate.h"

class EncryptedCertificate : public RawEncryptedCertificate {
	Q_DECLARE_TR_FUNCTIONS(EncryptedCertificate)
protected:
	bool attemptVerification(const RsaPublicKey& key);
	QSharedPointer<DecryptedCertificate> decryptedCertificate;
public:
	bool attemptVerificationFrom(const PlainCertificate& certificate);
	bool attemptVerificationFrom(const EncryptedCertificate& certificate);
	bool checkSignature(const RawData& signedData, const RawData& signature) const;
	EncryptedCertificate(const DataPointer& filewalker);
	virtual void printOn(Reporter& report) const;
	bool isVerified() const;
};

#else

class EncryptedCertificate : public RawEncryptedCertificate {
public:
	EncryptedCertificate(const DataPointer& filewalker) : RawEncryptedCertificate(filewalker){}
};

#endif

#endif
