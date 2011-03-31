#include "picgen.h"

QString svgGraph::drawBorderAndTimeAxis() const {
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

QString svgBarGraph::drawLegend() const{
	return QString(
		"\n\t\t<g style='text-anchor:left;font-size:16px;'>"
		"\n\t\t\t<rect x='730' y='86' fill='red' width='30' height='16' /><text x='765' y='100'>%1</text>"
		"\n\t\t\t<rect x='730' y='66' fill='blue' width='30' height='16' /><text x='765' y='80'>%2</text>"
		"\n\t\t\t<rect x='730' y='46' fill='yellow' width='30' height='16' /><text x='765' y='60'>%3</text>"
		"\n\t\t\t<rect x='730' y='26' fill='green' width='30' height='16' /><text x='765' y='40'>%4</text>"
		"\n\t\t</g>"
		)
		.arg(tr("= break/rest"))
		.arg(tr("= break, up to 15 min"))
		.arg(tr("= working"))
		.arg(tr("= driving"));
}

QString svgGraph::addSvgHull(QString content, int width) const { 
	return QString("\n<svg xmlns='http://www.w3.org/2000/svg' width='%1' height='135'>\n\t<g transform='translate(10,0)'>")
		.arg(width)
	+ content
	+ "\n\t</g>\n</svg>";
}