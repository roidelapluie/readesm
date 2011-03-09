#include "vuFactory.h"

#include "vuActivities.h"
#include "vuFaults.h"
#include "vuOverview.h"
#include "vuSpeeds.h"
#include "vuTechnical.h"

QSharedPointer<vuBlock> vuFactory(constDataPointer start) {
	typedef QSharedPointer<vuBlock> p;
	if(start[0] != 0x76) {
		qFatal("Not a block known to readesm, does not begin with 0x76");
	}
	switch(start[1]) {
		case vuOverview::TREP:
			return p(new vuOverview(start));
		case vuActivities::TREP:
			return p(new vuActivities(start));
		case vuFaults::TREP:
			return p(new vuFaults(start));
		case vuSpeeds::TREP:
			return p(new vuSpeeds(start));
		case vuTechnical::TREP:
			return p(new vuTechnical(start));
		default:
			qFatal("Oops! Unknown block %d", start[1]);
	}
	return p();
}
