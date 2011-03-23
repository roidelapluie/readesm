/** \file readesm.cpp
 \brief contains main() and handling of command line arguments.

 readesm mainly defines which files to read and parse. The actual parsing
 of files is described in esmfile.h, and the information about the parsing of
 the individual blocks just there.
 */


#include "config.h"
#include "fileformat/esmfile.h"
#include "fileformat/reporter/htmlReporter.h"
#include "fileformat/reporter/txtReporter.h"
#include "fileformat/reporter/xmlReporter.h"
#include "gui/mainWindow.h"

#include <QtCore/QByteArray>
#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>
#include <QtGui/QApplication>
template <typename T>
int main2(int argc, char** argv)
{
	T app(argc, argv);
	app.setApplicationName("readesm");
	app.setApplicationVersion(QString("%1 (%2)")
			.arg(VERSION)
			.arg(VERSION_DATE));
	app.setOrganizationName(QString::fromUtf8("Andreas Gölzer"));
	app.setOrganizationDomain("http://andreas.goelzer.de");
	
	QTextStream cerr(stderr);

	QTranslator qtTranslator;
	if(!qtTranslator.load("readesm_" + QLocale::system().name(),":/")) {
		cerr << "Could not load internationalization files for your locale :(" << endl;
	}
	app.installTranslator(&qtTranslator);
	if(argc < 3) {
		mainWindow widgetMainWindow;
		widgetMainWindow.show();
		if(argc == 2) widgetMainWindow.openFile(QString(argv[1]));
		return app.exec();
	} else if(argc == 3) {
		QString inputFile(argv[1]);
		QString outputFile(argv[2]);
		
		esmfile esm(inputFile);
		//determine extension of output file
		QString extension(outputFile.section(".",-1).toLower());

		//create report into temporary array
		QByteArray outData;
		if(extension == "txt") {
			txtReporter rep;
			rep << esm;
			outData = rep.str().toLocal8Bit();
		} else if(extension == "html" || extension == "htm") {
			htmlReporter rep;
			rep << esm;
			outData = rep.str().toUtf8();
		} else if(extension == "xml" || extension == "xhtml") {
			xmlReporter rep;
			rep << esm;
			outData = rep.str().toUtf8();
		} else {
			cerr << "Format for output not recognized." << endl;
			return 1;
		}
		
		QFile out(outputFile);
		out.open(QIODevice::WriteOnly | QIODevice::Text);
		
		if(!out.isOpen()) {
			cerr << "Could not open file " << outputFile << " for output. dumping to stdout" << endl;
			QTextStream cout(stdout);
			cout << outData;
			return 1;
		} else out.write(outData);
	} else {
		cerr << "Way too many arguments. Syntax is readesm [input file] [output file]" << endl;
	}
	return 0;
}

int main(int argc, char** argv)
{
	if(argc == 3) return main2<QCoreApplication>(argc,argv);
	else return main2<QApplication>(argc,argv);
}
