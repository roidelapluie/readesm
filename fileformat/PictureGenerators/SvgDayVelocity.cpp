#include "SvgDayVelocity.h"

QString SvgDayVelocity::toString() const{
	return addSvgHull(drawVelocityDescription() + "<g transform='scale(0.0083333,-1) translate(0,-100)'><path style='stroke:#dd2200' d='M 0 0 L " + collected + "' /></g>" + drawBorderAndTimeAxis(), 800);
}

void SvgDayVelocity::add(int secondsSinceMidnight, int velocity){
	collector << secondsSinceMidnight << " " << velocity << "\n";
}

QString SvgDayVelocity::drawVelocityDescription(){
	return	"\n\t\t<g style='stroke:black;stroke-width:1px;'>"
		"\n\t\t\t<line x1='0' y1='40' x2='720' y2='40'/>"
		"\n\t\t\t<line x1='0' y1='20' x2='720' y2='20'/>"
		"\n\t\t</g>"
		"\n\t\t<g style='stroke:black;stroke-width:2px;'>"
		"\n\t\t\t<line x1='720' y1='40' x2='724' y2='40'/>"
		"\n\t\t\t<line x1='720' y1='20' x2='724' y2='20'/>"
		"\n\t\t\t<line x1='720' y1='100' x2='724' y2='100'/>"
		"\n\t\t</g>"
		"\n\t\t<g style='text-anchor:left;font-size:16px;'>"
		"\n\t\t\t<text x='725' y='100'>0 km/h</text>"
		"\n\t\t\t<text x='725' y='20'>80 km/h</text>"
		"\n\t\t\t<text x='725' y='40'>60 km/h</text>"
		"\n\t\t</g>";
}
