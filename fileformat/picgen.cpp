#include "picgen.h"

void drawDayOutlineStart(QTextStream& o)
{
	o << "<svg xmlns='http://www.w3.org/2000/svg' width='740' height='120'>";
	o << "<g transform='translate(10,0)'>";
	o << "<g style='text-anchor:middle;font-size:16px;'>";
	for(int j = 0; j < 25; ++j)
		o << "<text x='" << (j * 30) << "' y='118'>" << j
				<< "</text><line x1='" << (j * 30) << "' y1='100' x2='" << (j
				* 30) << "' y2='104' style='stroke-width:2;stroke:black' />";
	o << "</g>";
	o << "<polyline points='0,0 720,0 720,100 0,100 0,0' style='fill:none;stroke:black;stroke-width:2'/>";
}

void drawDayOutlineEnd(QTextStream& o)
{
	o << "</g></svg>";
}
