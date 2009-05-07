/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef PICGEN_H
#define PICGEN_H PICGEN_H
#include "typedefs.h"

class picgen : public ostringstream{
	public:
	virtual string str(){ return ostringstream::str(); }
	virtual void add(int from, int duration, int height, string color, string title) {}
};

class htmlBarGraph : public picgen {
	static const int compressh = 2;
	public:
	virtual void add(int from, int duration, int height, string color, string title) {
		(*this) << "<img src='images/" << color <<".gif' width='" << (duration / compressh) << "' height='" << height << "' title='" << title << "' alt='" << title << "'/>";
	}
	virtual string str(){
		ostringstream o;
		o << ostringstream::str() << "<img src='images/scale.gif' height='20' width='" << (1440 / compressh) << "' alt='scale' />";
		return o.str();
	}
};

#endif
