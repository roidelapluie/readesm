#ifndef DRIVERACTIVITYDATA_H
#define DRIVERACTIVITYDATA_H

#include "RawCardDriverActivity.h"

class CardDriverActivity : public RawCardDriverActivity {
public:
	CardDriverActivity(const constDataPointer& start) : RawCardDriverActivity(start) {}
	
};

#endif
