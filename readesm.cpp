/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */

/** \file readesm.cpp
\brief contains main() and handling of command line arguments.

readesm mainly defines which files to read and parse. The actual parsing of files is described in esmfile.h, and the information about the parsing of the individual blocks just there.
*/
#include <iostream>
#ifndef HAVE_NO_BOOST
#include <boost/program_options.hpp>
#endif
#include "i18n.h"
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




int main(int argc, char* argv[]){
	i18nInit();

	string infile(""), outfile("-"), format("text");
	bool verbose(false);
#ifndef HAVE_NO_BOOST
	namespace po = boost::program_options;

	po::options_description desc(tr("ESM Reader, allowed options"));
	desc.add_options()
	("help,h", trc("produce help message"))
	("infile,i", po::value<string>(), trc("input file"))
	("outfile,o", po::value<string>()->default_value("-"), trc("output file"))
	("verbose,v",po::value<bool>()->default_value(false),trc("verbose output format, print simply everything"))
	("format,f", po::value<string>()->default_value("text"), trc("output format: text, html or xml"))
	;
	
	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
	} catch(po::unknown_option error) {
		std::cerr << error.what() << std::endl;
		std::cout << desc << std::endl;
		return 2;
	}
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}
	if (!vm.count("infile")) {
		std::cout << tr("Needs an input filename (--infile)!\n") << desc << std::endl;
		return 1;
	} else {
		infile = vm["infile"].as<string>();
	}
	format = vm["format"].as<string>();
	outfile = vm["outfile"].as<string>();
	verbose = vm["verbose"].as<bool>();
#else
	if(argc >= 2) infile = argv[1];
	else std::cerr << "Missing option.\nSyntax: readesm (input file) [output file] [format]";
	if(argc >= 3) outfile = argv[2];
	if(argc >= 4) format = argv[3];
	
#endif
	esmfile f(infile);
	reporter* report;
	if(format == "html") report = new htmlreporter(f.name());
	else if(format == "xml") report = new xmlreporter(f.name());
	else report = new txtreporter(f.name());
	report->verbose = verbose;
	*report << f;

	if (outfile == "-") {
		std::cout << report->str();
		return 0;
	} else {
		if(!slurptofile(outfile,  report->str())){
			std::cerr << tr("Could not open output file, writing to stdout");
			std::cout << report->str();
			return 2;
		} else {
			std::cout << f.name();
		}
	}
	
}
