/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef ESMFILEHEAD_H
#define ESMFILEHEAD_H
#include <string>
#include "slurpedfile.h"
#include "crypto.h"
#include "time.h"



class esmfilehead{
	public:
	slurpedfile content;
	string title;
	Time first;
	Time last;
	boost::shared_ptr<verifiedcert> CAcert;
	boost::shared_ptr<verifiedcert> devicecert;

	void reportDate(const Time& torep){
		if(torep < first) first = torep;
		if(torep > last) last = torep;
	}
	esmfilehead(const string& filename) :
		content(slurp(filename)),
		first(0x7fFfFfFf),
		last(0)
		{}
};


#endif
