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

#ifndef ESMFILE_H
#define ESMFILE_H
#include <vector>
#include <string>
#include <iostream>
#include "esmfilehead.h"
#include "reporter.h"
#include "block.h"
#include "cardblocks.h"
#include "vufactory.h"
#include "typedefs.h"
#include "time.h"

block::ptr Factory(iter start) {
	if(start[0] == 0x76) return block::ptr(vuFactory(start));
	else return tlvblock::Factory(start);
}

class esmfile : public esmfilehead {
	public:
	typedef std::vector<block::ptr> subray;
	typedef subray::const_iterator subiter;
	string name() {
		return title + ", " + first.datestr() + " to " + last.datestr();
	}

	subray blocks;
	esmfile(const string& filename) :
		esmfilehead(filename) {
		iter filewalker = content.begin();
		while(filewalker < content.end()) {
			block::ptr p(Factory(filewalker));
			blocks.push_back(p);
			filewalker += p->size();
		}
		for(subiter i = blocks.begin(); i < blocks.end(); ++i)
			(*i)->reportstuff(*this);
#ifndef HAVE_NO_CRYPTO
		if(CAcert && devicecert) {
			string filename("EC_PK.bin");
			if(!file_exists(filename)) filename
					= PREFIX "/share/readesm/EC_PK.bin";
			if(file_exists(filename)) {
				if(CAcert->verify(filename)) if(devicecert->verify(*CAcert)) for(subiter
						i = blocks.begin(); i < blocks.end(); ++i)
					(*i)->checksig(devicecert->key);
			} else {
				std::cerr
						<< "Cannot verify certificates and signatures: European main certificate file not found or not openable.";
			}
		}
#endif
	}
	friend reporter& operator<<(reporter& report, const esmfile& e) {
		report.bigblockstart("Statistics");
		report.single(tr("Statistics for") + " " + e.title, true);
		report.single(formatRange(e.first, e.last));
		report("Recorded days", e.daycount);
		report("Overall driven distance", stringify(e.drivenkm) + " km");
		report("Overall driving time", formatMinutes(e.drivenminutes));
		report("Average distance per day", stringify(e.drivenkm / e.daycount)
				+ " km");
		report("Average time driven per day", formatMinutes(e.drivenminutes
				/ e.daycount) + " (" + stringify(100* e .drivenminutes
				/ (e.daycount * 24* 60 ))+ "%)");
		report("Average speed when driving", stringify(e.drivenkm * 60 / e.drivenminutes) + " km/h");
		report.bigblockend();

		for(subiter i = e.blocks.begin(); i < e.blocks.end(); ++i) report << **i;
		return report;
	}
					};

#endif
