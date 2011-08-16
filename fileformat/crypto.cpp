#include "config.h"
#ifndef HAVE_NO_CRYPTO
#include "crypto.h"

#include <gcrypt.h>

#include <QtCore/QDebug>
#include <QtCore/QFile>

verifiedcert::verifiedcert(constDataPointer start_) :
	start(start_), car(start + 186), verified(false) {
}
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
}
bool verifiedcert::verify(const verifiedcert& other) {
	return other.verified && verify(other.key);
}
bool verifiedcert::verify(const rsa& extkey) {
	std::vector<unsigned char> buffer = extkey.perform(start.toUnsignedPointer(0), 128);
	copy(&buffer[1], &cdash[0], 106);
	copy(start.toUnsignedPointer(128), &cdash[106], 58);
	hash.resize(20);
	copy(&buffer[107], &hash[0], 20);
	if(!checkSHA1match(cdash, 164, &hash[0])) {
		qDebug() << "Certificate is invalid.\n";
		return false;
	}
	key = rsa(&cdash[28], 128, &cdash[156], 8);
	verified = true;
	return true;
}

reporter& operator<<(reporter& o, const verifiedcert& p) {
	o("SHA1", hexchunk(p.hash, 20));
	o("Key Identifier", hexchunk(&p.cdash[20], 8));
	o.blockstart("Certificate authority reference", 1);
	o << p.car;
	o.blockend();
	o("RSA public key", p.key.str());
	return o;
}

bool checkSHA1match(const unsigned char* text, int textlength,
		const unsigned char* hash) {
	std::vector<unsigned char> buffer(20);
	gcry_md_hash_buffer(GCRY_MD_SHA1, &buffer[0], text, textlength);
	bool cmp = true;
	for(int j = 0; cmp && j < 20; ++j) {
		cmp = (hash[j] == buffer[j]);
	}
	return cmp;
}

bool CheckSignature(const constDataPointer& data, int length, const constDataPointer& signature,
		int siglength, const rsa& key) {
	std::vector<unsigned char> decryptedsig = key.perform(signature.toUnsignedPointer(), siglength);
	bool valid = checkSHA1match(data.toUnsignedPointer(), length, &decryptedsig[107]);
	const unsigned char der[] = { 0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b,
			0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14 };
	valid = valid && compare(&decryptedsig[92], der, 15);
	for(int j = 1; j < 91; ++j)
		if(decryptedsig[j] != 0xff) valid = false;
	//not checking the first two, l207 p.251 says 0x00, 0x01,
	//but the files actually contain 0x01, 0xff
	if(!valid) {
		qDebug() << "\nsignature check failed ";
	}
	return valid;
}
#endif
