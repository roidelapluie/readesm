/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef RSA_H
#define RSA_H RSA_H

#include <gmpxx.h>
#include <vector>
///simple rsa, no padding or fancy stuff, just for the requirements this tacho stuff gives
class rsa {
	public:
	rsa() {};
	//rsa(mpz_class n_, mpz_class e_) : n(n_), e(e_)
	rsa(unsigned char* modulus, int mlen, unsigned char* exponent, int elen){
		mpz_import(n.get_mpz_t(), mlen, 1, 1, 0, 0, modulus);
		mpz_import(e.get_mpz_t(), elen, 1, 1, 0, 0, exponent);
	}
	std::vector<unsigned char> perform(const unsigned char* input, int length) const{
		mpz_class in, out;
		mpz_import(in.get_mpz_t(), length, 1, 1, 0, 0, input);
		mpz_powm(out.get_mpz_t(), in.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());
		size_t size;
		std::vector<unsigned char> rv(mpz_sizeinbase(out.get_mpz_t(), 256) + 10);
		mpz_export(&rv[0], &size, 1, 1, 0, 0, out.get_mpz_t());
		rv.resize(size);
		return rv;
	}
	private:	
	mpz_class n, e;
};

#endif
