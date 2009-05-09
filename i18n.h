/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef I18N_H
#define I18N_H I18N_H

#include <libintl.h>
#include <locale.h>
#include "typedefs.h"

void i18nInit(){
	setlocale( LC_ALL, "" );
	bindtextdomain( "readesm", "/home/goelzera/Documents/Programme/cpp/readesm/locale" );
	textdomain( "readesm" );
}

string tr(string in){
	return (in == "") ? "" : gettext(in.c_str());
}

string tr(const char* in){
	return *in == 0 ? "" : gettext(in);
}
#endif
