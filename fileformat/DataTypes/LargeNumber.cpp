#include "config.h"
#ifdef HAVE_CRYPTO

#include "LargeNumber.h"

LargeNumber::LargeNumber(const DataPointer& start, int length) {
	mpz_init(t);
	mpz_import(t, length, 1, 1, 0, 0, start.toUnsignedPointer());
}

LargeNumber::LargeNumber() {
	mpz_init(t);
}

LargeNumber::~LargeNumber() {
	mpz_clear(t);
}

LargeNumber::LargeNumber(const LargeNumber& o) {
	mpz_init_set(t, o.t);
}

LargeNumber& LargeNumber::operator=(const LargeNumber& o) {
	mpz_init_set(t, o.t);
	return *this;
}

QString LargeNumber::toString() const {
	return mpz_get_str(NULL, 10, t);
}

#endif
