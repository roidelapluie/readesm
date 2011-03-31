#include "xmlReporter.h"

#include <QtCore/QTextStream>

QString xmlReporter::str() {
	QString rv;
	QTextStream o(&rv); 
	o << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
		"\n<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n"
		"\n<head>\n\t<title>" << title << "</title>"
		"\n\t<meta http-equiv='Content-Type' content='text/html; charset=utf8'/>"
		"\n\t<style type=\"text/css\">"
		"\n\t\ttable		{ border-collapse:collapse;  border-width:2px; border-style:solid; }"
		"\n\t\ttr 		{ border-top-width:1px; border-top-style:dashed; }"
		"\n\t\ttr:hover 	{border-style:outset; background-color:#eeeeee; }"
		"\n\t\t.center	{text-align:center; }"
		"\n\t\tth		{text-align:left;vertical-align:top;}"
		"\n\t\ttd,th.max,th.avg, th.min{text-align:left;border-left: 1px dotted;}"
		"\n\t\tcaption		{font-size:0.5em;}"
		"\n\t\tsmall		{font-size:0.8em;}"
		"\n\t</style>"
		"\n</head>\n<body>"
		"\n<h1>" << title << "</h1>" 
		<< links << "<hr/>"
		<< reporter::str() << "<hr/>" << InfoAppend(true)
		<< "\n</body>\n</html>\n";
	return rv;
}

xmlReporter::xmlReporter(const QString& title_) :
	htmlReporter(title_) 
{
}

xmlReporter::pgptr xmlReporter::getBarGraph() const {
	return pgptr(new svgBarGraph);
}

bool xmlReporter::hasPlotGraph() const {
	return true;
}

xmlReporter::pgptr xmlReporter::getPlotGraph() const {
	return pgptr(new svgPlotGraph);
}
