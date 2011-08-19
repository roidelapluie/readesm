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
#include "DataTypes/RawData.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>

/*
class signature {
	public:
	enum validity {UNCHECKED, INVALID, VALID};
	bool present;
	validity valid;
	signature() : present(false), valid(UNCHECKED) {}
	void report(reporter& o) {
#ifndef HAVE_NO_CRYPTO
		if(valid == UNCHECKED) o("validly signed", "Could not or did not check signature");
		else if(valid == VALID) o("validly signed", "Signature checked and valid");
		else if(valid == INVALID) o("validly signed", "Signature checked and invalid");
		else o("error", "Sig state weird");
#endif
	}
};*/

class Block {
	Q_DECLARE_TR_FUNCTIONS(Block)
	public:
	Block(const constDataPointer& filewalker) :
		validsignature(false),
		hassignature(false),
		start(filewalker)
	{
	}


	friend reporter& operator<<(reporter& o, const Block& b) {
		o.bigblockstart(b.name());
		b.printOn(o);
/*#ifdef HAVE_CRYPTO
		o.single(b.hassignature ? (b.validsignature
				? tr("Block has valid signature") : tr("Beware: Block has invalid signature")) : tr("Block not signed"));
#else
		o.single(tr("no crypto support compiled in"));
#endif*/
		o.bigblockend();
		return o;
	}

	virtual int size() const = 0;

	virtual QString name() const {
		return tr("Unknown block type 0x%1").arg(readBigEndianInt2(start), 8, 16, QChar('0'));
	}

/*#ifdef HAVE_CRYPTO
	virtual bool checksig(const rsa& key) = 0;
#endif*/
	protected:
	virtual void printOn(reporter& o) const {
		o("length", size());
	}
	virtual RawData signedBytes() const = 0;

	protected:
	constDataPointer signature;
	bool validsignature, hassignature;
	constDataPointer start;
};

#endif
