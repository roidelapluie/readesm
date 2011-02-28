#include "txtReporter.h"

txtReporter::txtReporter(const QString& title_) :
	reporter(title_)
{
}

QString txtReporter::str() {
	return title + reporter::str() + InfoAppend();
}

void txtReporter::blockstart(const QString& description, int blockcount) {
	collector << "***************" << description << " (" << blockcount << ") *********\n";
}

void txtReporter::blockend() {
	collector << "*************************************\n";
}

void txtReporter::blockbreak() {
	collector << "  ***   \n";
}

void txtReporter::single(const QString& description, bool ishead) {
	QString mark = "";
	if(ishead) mark = "**";
	collector << mark << " " << description << " " << mark << endl;
}

void txtReporter::operator()(const QString& description, const QString& value) {
	collector << description << ": \t" << value << endl;
}

void txtReporter::operator()(const QString& description, int value) {
	collector << description << ": \t" << value << endl;
}

void txtReporter::bigblockstart(const QString& name) {
	collector << "\n\n+++++++++++++++++ Block: " << name
		<< " ++++++++++++++++++++++++++\n";
}

void txtReporter::bigblockend() {
	collector << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
}
