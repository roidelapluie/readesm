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
#ifndef VUFACTORY_H
#define VUFACTORY_H

#include "typedefs.h"
#include "Activities.h"
#include "Speeds.h"
#include "Technical.h"
#include "Faults.h"
#include "Overview.h"

shared_ptr<vublock> vuFactory(iter start) {
	typedef shared_ptr<vublock> p;
	if(start[0] != 0x76) throw std::runtime_error("Not a block");
	switch(start[1]) {
		case Overview::TREP:
			return p(new Overview(start));
		case Activities::TREP:
			return p(new Activities(start));
		case Faults::TREP:
			return p(new Faults(start));
		case Speeds::TREP:
			return p(new Speeds(start));
		case Technical::TREP:
			return p(new Technical(start));
		default:
			std::cerr << "Oops! " << start[1] << std::endl;
			throw std::runtime_error("Unknown block");
	}
	return p();
}

#endif
