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

#ifndef CRYPTO_H
#define CRYPTO_H CRYPTO_H
#include "config.h"

#ifndef HAVE_CRYPTO
#error "including crypto"
#endif

#include "formatStrings.h"
#include "helper.h"
#include "readTypes.h"
#include "reporter/reporter.h"
#include "rsa.h"
#include "DataTypes/CertificateAuthority.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QSharedPointer>

#include <vector>

bool checkSHA1match(const unsigned char* text, int textlength, const unsigned char* hash);


class verifiedcert {
	Q_DECLARE_TR_FUNCTIONS(verifiedcert)
	public:
	constDataPointer start;
	CertificateAuthority car;
	bool verified;
	rsa key;
	unsigned char cdash[164];
	std::vector<unsigned char> hash;
	typedef QSharedPointer<verifiedcert> ptr;
	verifiedcert(constDataPointer start_);
	bool verify(const QString& filename);
	bool verify(const verifiedcert& other);
	bool verify(const rsa& extkey);
	friend reporter& operator<<(reporter& o, const verifiedcert& p);
};

bool CheckSignature(const constDataPointer& data, int length, const constDataPointer& signature,
		int siglength, const rsa& key);
#endif
