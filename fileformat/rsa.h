/* Copyright 2009 Andreas Gölzer

 This file is part of readESM.

 readESM is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 readESM is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 readESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef RSA_H
#define RSA_H RSA_H

#include <gmp.h>
#include <vector>
#include <QString>

///wrap around mpz_t
/** This can also be done by the c++-bindings of GMP, but they are not yet
 * finished and not available everywhere */
class mpz {
	public:
	mpz_t t;
	int dummy;
	mpz() {
		mpz_init(t);
	}
	~mpz() {
		mpz_clear(t);
	}
	mpz(const mpz& o) {
		mpz_init_set(t, o.t);
	}
	mpz& operator=(const mpz& o) {
		mpz_init_set(t, o.t);
		return *this;
	}
	QString str() const{
		return mpz_get_str(NULL, 10, t);
	}
};

/* simple rsa, no padding or fancy stuff, just for the requirements this tacho
 stuff gives*/
class rsa {
	public:
	rsa() {
	}
	;
	rsa(const unsigned char* modulus, int mlen, const unsigned char* exponent, int elen) {
		mpz_import(n.t, mlen, 1, 1, 0, 0, modulus);
		mpz_import(e.t, elen, 1, 1, 0, 0, exponent);
	}
	std::vector<unsigned char> perform(const unsigned char* input, int length) const {
		mpz in, out;
		mpz_import(in.t, length, 1, 1, 0, 0, input);
		mpz_powm(out.t, in.t, e.t, n.t);
		size_t size;
		std::vector<unsigned char> rv(mpz_sizeinbase(out.t, 256) + 10);
		mpz_export(&rv[0], &size, 1, 1, 0, 0, out.t);
		rv.resize(size);
		return rv;
	}
	QString str() const {
		return QString("n = %1, e = %2").arg(n.str()).arg(e.str());
	}
	private:
	mpz n, e;
};

#endif
