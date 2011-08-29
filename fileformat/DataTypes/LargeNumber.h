#ifndef LARGENUMBER_H
#define LARGENUMBER_H

#include "config.h"
#ifdef HAVE_CRYPTO

#include <gmp.h>
#include "../DataPointer.h"

#include <QtCore/QString>

///wrap around mpz_t
/** This can also be done by the c++-bindings of GMP, but they are not yet
 * finished and not available everywhere */
class LargeNumber {
	public:
	mpz_t t;
	LargeNumber();
	~LargeNumber();
	LargeNumber(const DataPointer& start, int length);
	LargeNumber(const LargeNumber& o);
	LargeNumber& operator=(const LargeNumber& o);
	QString toString() const;
};

#else

#include "../DataPointer.h"
#include <QtCore/QString>

class LargeNumber {
public:
	LargeNumber(const DataPointer& start, int length){}
	QString toString() const{ return "not implemented";}
};

#endif

#endif
