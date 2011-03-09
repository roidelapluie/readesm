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

#ifdef HAVE_NO_CRYPTO
#error "including crypto"
#endif

#include "formatStrings.h"
#include "helper.h"
#include "readTypes.h"
#include "reporter/reporter.h"
#include "rsa.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QSharedPointer>

#include <vector>

bool checkSHA1match(const unsigned char* text, int textlength, const unsigned char* hash);

class CAid {
	Q_DECLARE_TR_FUNCTIONS(CAid)
	public:
	int numNation;
	QString alphaNation;
	int serialNumber;
	int additionalCoding;
	int identifier;
	bool operator==(const CAid& other) const {
		return numNation == other.numNation 
			&& alphaNation == other.alphaNation
			&& serialNumber == other.serialNumber 
			&& additionalCoding == other.additionalCoding 
			&& identifier == other.identifier;
	}
	bool operator!=(const CAid& other) const {
		return !operator==(other);
	}
	CAid(constDataPointer start) :
		numNation(start[0]), 
		alphaNation(fixedString(start + 1, 3)),
		serialNumber(start[4]), 
		additionalCoding(BEInt16(start + 5)),
		identifier(start[7]) 
	{
	}

	friend reporter& operator<<(reporter& o, const CAid& p) {
		o(tr("nationNumeric"), formatStrings::nationNumeric(p.numNation));
		o(tr("nationAlpha"), p.alphaNation);
		o(tr("keySerialNumber"), p.serialNumber);
		o(tr("additionalInfo"), p.additionalCoding);
		o(tr("caIdentifier"), p.identifier);
		return o;
	}
};

class verifiedcert {
	public:
	constDataPointer start;
	CAid car;
	bool verified;
	rsa key;
	typedef QSharedPointer<verifiedcert> ptr;
	verifiedcert(constDataPointer start_) :
		start(start_), car(start + 186), verified(false) {
	}
	bool verify(const QString& filename) {
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly)) return false;
		QByteArray rawkey = file.read(1e9);
		file.close();
		constDataPointer keyPointer(rawkey);
		CAid rawkey_ca(keyPointer);
		if(rawkey_ca != car) {
			qDebug() << "Attempting to use wrong ca certificate.";
			return false;
		}
		return verify(rsa(keyPointer.toUnsignedPointer(8), 128, keyPointer.toUnsignedPointer(136), 8));
	}
	bool verify(const verifiedcert& other) {
		return other.verified && verify(other.key);
	}
	bool verify(const rsa& extkey) {
		std::vector<unsigned char> buffer = extkey.perform(start.toUnsignedPointer(0), 128);
		unsigned char cdash[164];
		copy(&buffer[1], &cdash[0], 106);
		copy(start.toUnsignedPointer(128), &cdash[106], 58);
		if(!checkSHA1match(cdash, 164, &buffer[107])) {
			qDebug() << "Certificate is invalid.\n";
			return false;
		}
		key = rsa(&cdash[28], 128, &cdash[156], 8);
		verified = true;
		return true;
	}
	friend reporter& operator<<(reporter& o, const verifiedcert& p) {
		return o << p.car;
	}
};

bool CheckSignature(const constDataPointer& data, int length, const constDataPointer& signature,
		int siglength, const rsa& key);
#endif
