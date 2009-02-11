/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */

/** \file readesm.cpp
\brief contains main() and handling of command line arguments.

readesm mainly defines whichh files to read and parse. The actual parsing of files is described in esmfile.h, and the information about the parsing of the individual blocks just there.
*/
#include <iostream>
#include <boost/program_options.hpp>
#include "esmfile.h"
#include "reporter.h"

/**
\mainpage readesm - Digital Tachograph file reader


\section intro_scope Uses
This program takes an ESM file, downloaded from a digital tachograph or a driver card, and converts it into human-readable form, either html or simple plaintext. Since the data format is taken from an european community regulation (OJ:L:2002:207, p. 1-252, http://eur-lex.europa.eu/LexUriServ/LexUriServ.do?uri=OJ:L:2002:207:0001:0252:EN:PDF), i doubt it is of any use for drivers or companies outside of the european union.

\section intro_comp Compiling
Since so far there is no binary distribution for the program, you will have to compile it yourself.
To do that, you need a c++ compiler, parts of the boost (http://www.boost.org/) library (specifically program_options and shared_ptr) and the Gnu MP library (http://gmplib.org/), wich is used to check the various RSA signatures. Once all those are installed, typing make/make install should work fine.

\section intro_run Running the program
In most circumstances, you will run the program from the commandline: readesm --infile foo.esm --outfile bar.html
Alternatively, if no output file is specified, stdout is used.

*/



namespace po = boost::program_options;

int main(int argc, char* argv[]){
	po::options_description desc("ESM Reader, allowed options");
	desc.add_options()
	("help", "produce help message")
	("infile", po::value<std::string>(), "input file")
	("outfile", po::value<std::string>(), "output file")
	("verbose",po::value<bool>()->default_value(false),"verbose output format, print just everything")
	("format", po::value<std::string>()->default_value("text"), "output format, text or html")
	;
	
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 0;
	}
	if (!vm.count("infile")) {
		std::cout << "Needs an input filename (--infile)!\n" << desc << "\n";
		return 1;
	}

	esmfile f(vm["infile"].as<std::string>());
	reporter* report;
	if(vm["format"].as<std::string>() == "html") report = new htmlreporter(f.name());
	else report = new txtreporter(f.name());
	report->verbose = vm["verbose"].as<bool>();
	*report << f;

	if (!vm.count("outfile") || vm["outfile"].as<std::string>() == "-") {
		std::cout << report->str();
		return 0;
	} else {
		if(!slurptofile(vm["outfile"].as<std::string>(),  report->str())){
			std::cerr << "Could not open output file, writing to stdout";
			std::cout << report->str();
			return 2;
		} else {
			std::cout << f.name();
		}
	}
	
}
