#ifndef RSAPUBLICKEY_H
#define RSAPUBLICKEY_H

#include "RawRsaPublicKey.h"
#include <vector>

class RsaPublicKey : public RawRsaPublicKey {
	Q_DECLARE_TR_FUNCTIONS(RsaPublicKey)
public:
	RsaPublicKey(const constDataPointer& start) : RawRsaPublicKey(start) {}
	QByteArray perform(const RawData& input) const {
		LargeNumber in, out;
		mpz_import(in.t, input.length(), 1, 1, 0, 0, input.toUnsignedPointer());
		mpz_powm(out.t, in.t, e.t, n.t);
		QByteArray rv(mpz_sizeinbase(out.t, 256) + 10, 0);
		size_t size;
		mpz_export(rv.data(), &size, 1, 1, 0, 0, out.t);
		rv.resize(size);
		return rv;
	}

};
#endif
