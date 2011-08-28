#include "SvgDayActivity.h"

QString SvgDayActivity::toString() const{
	return addSvgHull("\n\t\t<g transform='scale(0.5,-1) translate(0,-100)'>" + collected + "\n\t\t</g>" + drawBorderAndTimeAxis() + drawLegend(), 900);
}

void SvgDayActivity::add(int from, int duration, int height, const QString& color, const QString& title){
	collector<< "\n\t\t\t<rect x='" << from << "' fill='" << color << "' width='"
			<< duration << "' height='" << height << "'><title>"<< title << "</title></rect>";
}

QString SvgDayActivity::drawLegend(){
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