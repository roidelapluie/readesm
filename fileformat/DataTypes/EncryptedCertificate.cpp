#include "EncryptedCertificate.h"

#include "../checkSha1.h"

/*
#include <QtCore/QDebug>
*/

EncryptedCertificate::EncryptedCertificate(const constDataPointer& filewalker) : RawEncryptedCertificate(filewalker),
	decryptedCertificate()
{}

bool EncryptedCertificate::attemptVerification(const RsaPublicKey& key){
	QByteArray srdash = key.perform(sign);
	if(srdash.size() != 128 || srdash.at(0) != 0x6A || (unsigned char)srdash[127] != 0xBC) return false;
	QByteArray crdash = srdash.mid(1, 106);
	QByteArray hdash = srdash.mid(107, 20);
	QByteArray cdash = crdash.append(cndash.toQByteArray()); // implicitly shared, but we do not need crdash anymore
	if(!checkSha1(cdash, hdash)) return false;
	decryptedCertificate = QSharedPointer<DecryptedCertificate>(new DecryptedCertificate(constDataPointer(cdash)));
	return true;
}

bool EncryptedCertificate::checkSignature(const RawData& signedData, const RawData& signature) const{
	if(!isVerified()) return false;
	QByteArray srdash = decryptedCertificate->rsaPublicKey.perform(signature);
	QByteArray hdash = srdash.mid(107, 20);
	if(!checkSha1(signedData, hdash)) return false;
	const unsigned char der[] = { 0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b,
			0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14 };
	for(int j = 0; j < 15; ++j)
		if((unsigned char)srdash.at(92 + j) != der[j]) return false;
	for(int j = 1; j < 91; ++j)
		if((unsigned char)srdash.at(j) != 0xff) return false;
	//not checking the first two, l207 p.251 says 0x00, 0x01,
	//but the files actually contain 0x01, 0xff
	return true;
}

bool EncryptedCertificate::isVerified() const {
	return decryptedCertificate;
}

void EncryptedCertificate::printOn(reporter& report) const {
	if(decryptedCertificate){
		decryptedCertificate->printOn(report);
		report.single(tr("Certificate verified from:"));
		certificateAuthorityReference.printOn(report);
	} else {
		report.single(tr("Unverified certificate, needs verification from:"));
		certificateAuthorityReference.printOn(report);
	}
}

bool EncryptedCertificate::attemptVerificationFrom(const PlainCertificate& certificate){
	return certificateAuthorityReference == certificate.keyIdentifier && attemptVerification(certificate.rsaPublicKey);
}

bool EncryptedCertificate::attemptVerificationFrom(const EncryptedCertificate& other){
	return other.isVerified() && attemptVerification(other.decryptedCertificate->rsaPublicKey);
}

/*
bool verifiedcert::verify(const QString& filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) return false;
	QByteArray rawkey = file.readAll();
	file.close();
	constDataPointer keyPointer(rawkey);
	CertificateAuthority rawkey_ca(keyPointer);
	if(rawkey_ca != car) {
		qDebug() << "Attempting to use wrong ca certificate.";
		return false;
	}
	return verify(rsa(keyPointer.toUnsignedPointer(8), 128, keyPointer.toUnsignedPointer(136), 8));
}*/