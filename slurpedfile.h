/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef SLURPEDFILE_H
#define SLURPEDFILE_H SLURPEDFILE_H
#include <vector>
#include <fstream>
#include <stdexcept>
#include <string>
#include "typedefs.h"

class file_not_found : public std::runtime_error {
	public: file_not_found(string s) : std::runtime_error(s) {}
};

bool file_exists(const string& filename){
	std::ifstream f(filename.c_str());
	return f.is_open();
}

slurpedfile slurp(const string& filename){
	slurpedfile fbuffer;
	std::ifstream f(filename.c_str(),std::ios::binary);
	if(!f.is_open()){
		throw file_not_found("error opening file " + filename);
	}
	
	//get file size
	int begin = f.tellg();
	f.seekg (0, std::ios::end);
	int end = f.tellg();
	f.seekg (0, std::ios::beg);
	fbuffer.resize(end - begin);
	f.read(reinterpret_cast<char*>(&fbuffer[0]),fbuffer.size());
	f.close();
	return fbuffer;
}



bool slurptofile(const string& filename, const std::string& content){
	std::ofstream f(filename.c_str(),std::ios::binary);
	if(!f.is_open()) return false;
	f << content;
	f.close();
	return true;
}

#endif
