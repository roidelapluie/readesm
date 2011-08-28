#ifndef VUACTIVITIES_H
#define VUACTIVITIES_H

#include "RawVuActivities.h"

class VuActivities : public RawVuActivities {
	Q_DECLARE_TR_FUNCTIONS(VuActivities)
public:
	VuActivities(const DataPointer& filewalker);
	QString name() const;
};


#endif
