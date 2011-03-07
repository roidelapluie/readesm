/* Copyright 2009 Andreas Gölzer

 This file is part of readESM.

 readESM is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 readESM is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 readESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef PICGEN_H
#define PICGEN_H PICGEN_H

#include <QString>
#include <QTextStream>

class picgen{
	protected:
		QString collected;
	public:
		QTextStream collector;
	picgen() : collected(), collector(&collected) {}
	virtual QString str() {
		return collected;
	}
	virtual void add(int from, int duration, int height, QString color, QString title) {}
};

void drawDayOutlineStart(QTextStream& o);
void drawDayOutlineEnd(QTextStream& o);

class htmlBarGraph : public picgen {
	static const int compressh = 2;
	public:
	virtual void add(int from, int duration, int height, QString color, QString title) {
		collector<< "<img src='images/" << color << ".gif' width='" << (duration
				/ compressh) << "' height='" << height << "' title='" << title
				<< "' alt='" << title << "'/>";
	}
	virtual QString str() {
		QString rv;
		QTextStream o(&rv); 
		o << str()
				<< "<img src='images/scale.gif' height='20' width='" << (1440
				/ compressh) << "' alt='scale' />";
		return rv;
	}
};

class svgBarGraph : public picgen {
	public:
	virtual void add(int from, int duration, int height, QString color, QString title) {
		collector<< "<rect x='" << from << "' fill='" << color << "' width='"
				<< duration << "' height='" << height << "' title='" << title
				<< "'><title>"<< title << "</title></rect>\n";
	}
	virtual QString str() {
		QString rv;
		QTextStream o(&rv); 
		drawDayOutlineStart(o);
		o << "<g transform='scale(0.5,-1) translate(0,-100)'>"
				<< collected << "</g>";
		drawDayOutlineEnd(o);
		return rv;
	}
};

class svgPlotGraph : public picgen {
	public:
	virtual QString str() {
		QString rv;
		QTextStream o(&rv); 
		drawDayOutlineStart(o);
		o << "<g transform='scale(0.0083333,-1) translate(0,-100)'><path style='stroke:#dd2200' d='M 0 0 L "
		<< collected << "' /></g>";
		drawDayOutlineEnd(o);
		return rv;
	}
};

#endif
