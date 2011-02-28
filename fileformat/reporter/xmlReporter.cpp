#include "xmlReporter.h"

QString xmlReporter::str() {
	QString rv;
	QTextStream o(&rv); 
	o << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
		"<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n"
		"<head><title>" << title << "</title>"
		"<meta http-equiv='Content-Type' content='text/html; charset=utf8'/>"
		"<style type=\"text/css\">"
		"table		{ border-collapse:collapse;  border-width:2px; border-style:solid; }"
		"tr 		{ border-top-width:1px; border-top-style:dashed; }"
		"tr:hover 	{border-style:outset; background-color:#eeeeee; }"
		".center	{text-align:center; }"
		"th		{text-align:left;vertical-align:top;}"
		"td,th.max,th.avg, th.min{text-align:left;border-left: 1px dotted;}"
		"caption		{font-size:0.5em;}"
		"small		{font-size:0.8em;}</style>"
		"</head><body>"
		"<h1>" << title << "</h1>" 
		<< links << "<hr/>"
		<< reporter::str() << "<hr/>" << InfoAppend(true)
		<< "</body></html>\n";
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
