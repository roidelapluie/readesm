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

#ifndef BLOCK_H
#define BLOCK_H BLOCK_H

#include "constDataPointer.h"
#include "readTypes.h"
#include "reporter/reporter.h"
#include "DataTypes/EncryptedCertificate.h"
#include "DataTypes/RawData.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

class Block {
	Q_DECLARE_TR_FUNCTIONS(Block)
	public:
	Block(const constDataPointer& filewalker);
	friend reporter& operator<<(reporter& o, const Block& b);
	virtual int size() const = 0;
	virtual QString name() const;
	virtual void checkSignature(const EncryptedCertificate& cert);
	
	protected:
	virtual RawData signedBytes() const = 0;
	virtual RawData signatureBytes() const;
	virtual void printOn(reporter& o) const;
	bool validSignature;
	bool hasSignature;
	constDataPointer start;
};

#endif
