#include "SvgDayGraph.h"

QString SvgDayGraph::drawBorderAndTimeAxis(){
	QString rv;
	QTextStream o(&rv);
	o << "\n\t\t<g style='text-anchor:middle;font-size:16px;'>";
	for(int j = 0; j < 25; ++j)
		o << "\n\t\t\t<text x='" << (j * 30) << "' y='118'>" << j << "</text>";
	o << "\n\t\t\t<text x='360' y='132'>" << tr("Time of day (UTC)") << "</text>";
	o << "\n\t\t</g>";
	o << "\n\t\t<g style='stroke:black;stroke-width:2;'>";
	for(int j = 0; j < 25; ++j)
		o << "\n\t\t\t<line x1='" << (j * 30) << "' y1='100' x2='" << (j * 30) << "' y2='104'/>";
	o << "\n\t\t\t<polyline points='0,0 720,0 720,100 0,100 0,0' style='fill:none;'/>";
	o << "\n\t\t</g>";
	return rv;
}


QString SvgDayGraph::addSvgHull(const QString& content, int width){
	return QString("\n<svg xmlns='http://www.w3.org/2000/svg' width='%1' height='135'>\n\t<g transform='translate(10,0)'>")
		.arg(width)
	+ content
	+ "\n\t</g>\n</svg>";
}
