#ifndef VUOVERVIEW_H
#define VUOVERVIEW_H

#include "RawVuOverview.h"

class VuOverview : public RawVuOverview {
	Q_DECLARE_TR_FUNCTIONS(VuOverview)
public:
	VuOverview(const constDataPointer& filewalker);
	virtual RawData signedBytes() const;
};


#endif
