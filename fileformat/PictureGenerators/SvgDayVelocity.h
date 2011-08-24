#ifndef SVGDAYVELOCITY_H
#define SVGDAYVELOCITY_H

#include "SvgDayGraph.h"

class SvgDayVelocity : public SvgDayGraph {
public:
	virtual QString toString() const;
	///add a time-velocity pair. velocities should be added sorted with ascending time
	virtual void add(int secondsSinceMidnight, int velocity);
protected:
	static QString drawVelocityDescription();
};
#endif
