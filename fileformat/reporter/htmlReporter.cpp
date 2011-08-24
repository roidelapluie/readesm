#include "htmlReporter.h"

htmlReporter::htmlReporter(const QString& title_) :
	reporter(title_),
	links(),
	linksCollector(&links),
	targetcount(0)
{
}

QString htmlReporter::escapeString(QString in) const{
	in.replace("&","&amp;");
	return in;
}

QString htmlReporter::str() {
	QString rv;
	QTextStream o(&rv);
	o << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" dir=\"ltr\" lang=\"en-US\">\n<head><link rel='stylesheet' type='text/css' media='screen' href='style.css'/>"
	"<title>" << title << "</title>"
	"<meta http-equiv='Content-Type' content='text/html; charset=utf8'/></head><body>"
	"<h1>" << title << "</h1>"
	<< links << "<hr/>"
	<< reporter::str() << "<hr/>" << InfoAppend(true)
	<< "</body></html>\n";
	return rv;
}

void htmlReporter::bigblockstart(const QString& name) {
	linksCollector << "\n<a href='#" << (++targetcount) << "'>" << escapeString(name)
			<< "</a><br/>";
	collector << "\n<h2><a name='" << targetcount << "'>"
			<< escapeString(name) << "</a></h2>\n<table>";
}

void htmlReporter::bigblockend() {
	collector << "</table>\n";
}

void htmlReporter::blockstart(const QString& description, int blockcount) {
	collector << "<tr><th>" << escapeString(description) << "</th><td><table>\n";
}

void htmlReporter::blockbreak() {
	collector << "<tr><th></th><td></td></tr>\n";
}

void htmlReporter::blockend() {
	collector << "</table></td></tr>\n";
}

void htmlReporter::single(const QString& description, bool ishead) {
	QString mark = "td";
	if(ishead) mark = "th";
	collector << "<tr><" << mark << " colspan='2' class='center'>" <<
			escapeString(description) << "</" << mark << "></tr>" << endl;
}

void htmlReporter::operator()(const QString& description, const QString& value) {
	collector << "<tr><th>" << description << "</th><td>" << escapeString(value)
			<< "</td></tr>\n";
}

void htmlReporter::operator()(const QString& description, int value) {
	collector << "<tr><th>" << description << "</th><td>" << value
			<< "</td></tr>\n";
}

bool htmlReporter::hasBarGraph() const {
	return true;
}
