#ifndef SVGDAYACTIVITY_H
#define SVGDAYACTIVITY_H

#include "SvgDayGraph.h"

class SvgDayActivity : public SvgDayGraph {
	Q_DECLARE_TR_FUNCTIONS(SvgDayActivity)
public:
	///add an activity rectangle. from is minutes since midnight, duration is minutes, height pixels, color html code, and title text to show on mouseover
	virtual void add(int from, int duration, int height, const QString& color, const QString& title);
	virtual QString toString() const;
protected:
	static QString drawLegend();
};

#endif
