#include "config.h"
#ifndef HAVE_NO_CRYPTO
#include "crypto.h"
#include <gcrypt.h>

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