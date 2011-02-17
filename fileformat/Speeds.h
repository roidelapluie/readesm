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

#ifndef SPEEDS_H
#define SPEEDS_H SPEEDS_H
#include <string>
#include "vublock.h"

///See page 163 of l207.pdf
class Speeds : public vublock {
	public:
	string name() const {
		return "Detailed Speed";
	}
	static const int TREP = 0x4;
	Speeds(iter nstart) :
		vublock(nstart) {
		Init();
	}
	int size() const {
		return 2 + 64* Int16 (0) + 2 + 128;;
	}
	void CompleteReport(reporter& report) const {
		runningIndex = 0;
		for(reporter::subblock b = report.newsub("VuDetailedSpeedBlock",
				Int16()); b(); ++b) {
			report("speedBlockBeginDate", readDate().str());
			ostringstream o;
			for(int k = 0; k < 60; ++k)
				o << IntByte() << ", ";
			report("speedsPerSecond", o.str());
		}
	}
	void BriefReport(reporter& report) const {
		if(!report.hasPlotGraph()) {
			//this really requires a plotting program and stuff
			report("Speed data", "omitted");
			return;
		}
		runningIndex = 0;
		int count = Int16();
		if(!count) return;
		int j = 0;
		Time date = readDate();
		while(++j < count) {
			reporter::pgptr visual(report.getPlotGraph());
			Time sdate(date);
			int daystart = date.timestamp - date.timestamp % 86400;
			int dayend = daystart + 86400;
			*visual << (date.timestamp - daystart - 1) << " 0\n";
			do {
				int difference = date.timestamp - daystart;
				for(int k = 0; k < 60; ++k)
					*visual << (difference + k) << " " << IntByte()
							<< std::endl;
				if(j < count) {
					Time ndate = readDate();
					if(ndate.timestamp != date.timestamp + 60
							&& ndate.timestamp < dayend) {
						*visual << (difference + 60) << " 0\n";
						*visual << (ndate.timestamp - daystart - 1) << " 0\n";
					}
					date = ndate;
				}
			} while(++j < count && date.timestamp < dayend);
			report(sdate.datestr(), visual->str());
		}
	}

};

#endif
