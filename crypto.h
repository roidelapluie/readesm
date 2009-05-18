/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef CRYPTO_H
#define CRYPTO_H CRYPTO_H
#ifdef HAVE_NO_CRYPTO
#error "including crypto"
#endif

#include <vector>
#include <gcrypt.h>

#include "rsa.h"
#include "formatStrings.h"
#include "readTypes.h"
#include "reporter.h"
#include "helper.h"
#include "slurpedfile.h"
#include "typedefs.h"

bool checkSHA1match(const unsigned char* text, int textlength, const unsigned char* hash){
	std::vector<unsigned char> buffer(20);
	gcry_md_hash_buffer(GCRY_MD_SHA1, &buffer[0], text, textlength);
	bool cmp = true;
	for(int j = 0; cmp && j < 20; ++j){
		cmp = (hash[j] == buffer[j]);
	}
	return cmp;
}


class CAid{
	public:
	int numNation;
	string alphaNation;
	int serialNumber;
	int additionalCoding;
	int identifier;
	bool operator==(const CAid& other) const{
		return numNation == other.numNation && alphaNation == other.alphaNation && serialNumber == other.serialNumber && additionalCoding == other.additionalCoding && identifier == other.identifier;
	}
	bool operator!=(const CAid& other) const{ return !operator==(other); }
	CAid(iter start) :
		numNation(start[0]), 
		alphaNation(fixedString(start + 1, 3)), 
		serialNumber(start[4]),
		additionalCoding(BEInt16(start + 5)),
		identifier(start[7])
		{}
	friend reporter& operator<<(reporter& o, const CAid& p){
		o("nationNumeric", nationNumeric(p.numNation));
		o("nationAlpha", p.alphaNation);
		o("keySerialNumber", p.serialNumber);
		o("additionalInfo", p.additionalCoding);
		o("caIdentifier", p.identifier);
		return o;
	}
};

class verifiedcert{
	public:
	iter start;
	CAid car;
	bool verified;
	rsa key;
	typedef shared_ptr<verifiedcert> ptr;
	verifiedcert(iter start_) : start(start_), car(start + 186), verified(false) {}
	bool verify(const string& filename){
		slurpedfile rawkey = slurp(filename);
		CAid rawkey_ca(rawkey.begin());
		if(rawkey_ca != car){
			std::cerr << "Attempting to use wrong ca certificate.";
			return false;
		}
		return verify(rsa(&rawkey[8], 128, &rawkey[136], 8));
	}
	bool verify(const verifiedcert& other){
		return other.verified && verify(other.key);
	}
	bool verify(const rsa& extkey){
		std::vector<unsigned char> buffer = extkey.perform(&start[0], 128);
		unsigned char cdash[164];
		copy(&buffer[1], &cdash[0], 106);
		copy(&start[128], &cdash[106], 58);
		if(!checkSHA1match(cdash, 164, &buffer[107])){
			std::cout << "Certificate is invalid.\n";
			return false;
		}
		key = rsa(&cdash[28],128,&cdash[156],8);
		verified = true;
		return true;
	}
	friend reporter& operator<<(reporter& o, const verifiedcert& p){
		return o << p.car;
	}
};

bool CheckSignature(const iter& data, int length, const iter& signature, int siglength, const rsa& key){
	std::vector<unsigned char> decryptedsig = key.perform(&signature[0],siglength);
	bool valid = checkSHA1match(&data[0], length,&decryptedsig[107]);
	const unsigned char der[] = {0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14};
	valid = valid && compare(&decryptedsig[92],der,15);
	for(int j = 1; j < 91; ++j) if(decryptedsig[j] != 0xff) valid = false;
	//not checking the first two, l207 p.251 says 0x00, 0x01,
	//but the files actually contain 0x01, 0xff
	if(!valid){
		std::cerr << "\nsignature check failed ";
	}
	return valid;
}

#endif
