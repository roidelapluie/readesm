/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef SHA1_H
#define SHA1_H SHA1_H

#include <vector>
#include <gcrypt.h>

bool checkSHA1match(const unsigned char* text, int textlength, const unsigned char* hash){
	std::vector<unsigned char> buffer(20);
	gcry_md_hash_buffer(GCRY_MD_SHA1, &buffer[0], text, textlength);
	bool cmp = true;
	for(int j = 0; cmp && j < 20; ++j){
		cmp = (hash[j] == buffer[j]);
	}
	return cmp;
}

#endif
