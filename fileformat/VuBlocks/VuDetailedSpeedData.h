#ifndef VUDETAILEDSPEEDDATA_H
#define VUDETAILEDSPEEDDATA_H

#include "RawVuDetailedSpeedData.h"

class VuDetailedSpeedData : public RawVuDetailedSpeedData {
public:
	VuDetailedSpeedData(const constDataPointer& start) : RawVuDetailedSpeedData(start) {}
};

#endif
