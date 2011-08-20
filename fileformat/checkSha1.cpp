#include "checkSha1.h"

#include "config.h"
#ifndef HAVE_CRYPTO
#error "including libgcrypt-dependent file despite not having all crypto libs"
#endif

#include <gcrypt.h>

bool checkSha1(const RawData& data, const QByteArray& hash){
	QByteArray buffer(20, 0);
	gcry_md_hash_buffer(GCRY_MD_SHA1, buffer.data(), data.toUnsignedPointer(), data.length());
	for(int j = 0; j < 20; ++j) {
		if(hash[j] != buffer[j]) return false;
	}
	return true;
}
