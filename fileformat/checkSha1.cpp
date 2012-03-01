#include "checkSha1.h"

#include "config.h"
#ifdef HAVE_CRYPTO

#include <gcrypt.h>

bool checkSha1(const RawData& data, const QByteArray& hash) {
	QByteArray buffer(20, 0);
	gcry_md_hash_buffer(GCRY_MD_SHA1, buffer.data(), data.toUnsignedPointer(), data.size());
	for(int j = 0; j < 20; ++j) {
		if(hash[j] != buffer[j]) return false;
	}
	return true;
}

#endif
