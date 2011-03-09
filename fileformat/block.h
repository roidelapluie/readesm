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

#include "config.h"
#include "constDataPointer.h"
#include "esmfilehead.h"
#include "helper.h"
#include "readTypes.h"
#include "reporter/reporter.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

class signature {
	public:
	enum validity {UNCHECKED, INVALID, VALID};
	bool present;
	validity valid;
	signature() : present(false), valid(UNCHECKED) {}
	void report(reporter& o){
#ifndef HAVE_NO_CRYPTO
		if(valid == UNCHECKED) o("validly signed", "Could not or did not check signature");
		else if(valid == VALID) o("validly signed", "Signature checked and valid");
		else if(valid == INVALID) o("validly signed", "Signature checked and invalid");
		else o("error", "Sig state weird");
#endif
	}
};

class block {
	Q_DECLARE_TR_FUNCTIONS(block)
	public:
	typedef QSharedPointer<block> ptr;
	block(constDataPointer filewalker) :
		validsignature(false), 
		hassignature(false), 
		start(filewalker), 
		type(getType(start)) 
	{
	}

	static int getType(constDataPointer filewalker) {
		return BEInt16(filewalker);
	}

	friend reporter& operator<<(reporter& o, const block& b) {
		o.bigblockstart(b.name());
		b.printOn(o);
#ifndef HAVE_NO_CRYPTO
		o(tr("validly signed"), b.hassignature ? (b.validsignature
				? tr("yes, valid") : tr("no, invalid")) : tr("no, not signed"));
#else
		o(tr("validly signed"),tr("no crypto support compiled in"));
#endif
		o.bigblockend();
		return o;
	}
	
	virtual int size() const = 0;

	virtual QString name() const {
		return tr("Unknown block type %1").arg(hex(type, 4));
	}

	virtual void reportstuff(esmfilehead& esm) {
	}

	static ptr Factory(constDataPointer filewalker);
#ifndef HAVE_NO_CRYPTO
	virtual bool checksig(const rsa& key) = 0;
#endif
	protected:
	virtual void printOn(reporter& o) const {
		o("length", size());
	}

	protected:
	constDataPointer signature;
	bool validsignature, hassignature;
	constDataPointer start;
	int type;
};

#endif
