#ifndef LARGENUMBER_H
#define LARGENUMBER_H

#include <gmp.h>
#include "../constDataPointer.h"

#include <QtCore/QString>

///wrap around mpz_t
/** This can also be done by the c++-bindings of GMP, but they are not yet
 * finished and not available everywhere */
class LargeNumber {
	public:
	mpz_t t;
	LargeNumber();
	~LargeNumber();
	LargeNumber(const constDataPointer& start, int length);
	LargeNumber(const LargeNumber& o);
	LargeNumber& operator=(const LargeNumber& o);
	QString toString() const;
};

#endif
