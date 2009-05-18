/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
typedef std::vector<unsigned char> slurpedfile;
typedef slurpedfile::const_iterator iter;
using boost::shared_ptr;
using std::string;
using std::ostringstream;

#ifndef PREFIX
#define PREFIX "/usr/local"
#endif

#endif
