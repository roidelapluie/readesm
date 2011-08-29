#include "RsaPublicKey.h"
#include "config.h"
#ifdef HAVE_CRYPTO

RsaPublicKey::RsaPublicKey(const DataPointer& start) : RawRsaPublicKey(start) {}

QByteArray RsaPublicKey::perform(const RawData& input) const {
	LargeNumber in, out;
	mpz_import(in.t, input.size(), 1, 1, 0, 0, input.toUnsignedPointer());
	mpz_powm(out.t, in.t, e.t, n.t);
	QByteArray rv(mpz_sizeinbase(out.t, 256) + 10, 0);
	size_t size;
	mpz_export(rv.data(), &size, 1, 1, 0, 0, out.t);
	rv.resize(size);
	return rv;
}

#endif