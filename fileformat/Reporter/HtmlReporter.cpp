#include "HtmlReporter.h"

#include "../loadFile.h"
#include "../DataTypes/DataType.h"
#include "../TopLevelBlock.h"


void HtmlReporter::tagValuePair(const QString& tag, const QString& value){
	QString copy(value);
	copy.replace("&","&amp;");
	collector << "\n<li>" << tag << ": <b>" << copy << "</b></li>";
}

void HtmlReporter::namedSubBlock(const QString& tag, const DataType& value){
	collector << "\n<li>" << tag << ": <ul>";
	value.printOn(*this);
	collector << "</ul></li>";
}

void HtmlReporter::topLevelBlock(const TopLevelBlock& value){
	collector << "\n<h3>" << value.name() << "</h3>";
	collector << "<ul>";
	*this << value;
	collector << "</ul>";
}


QByteArray HtmlReporter::toQByteArray() const{
	QByteArray filecontent = loadFile(":/template.html");
	filecontent.replace("$title", title.toUtf8());
	filecontent.replace("$content", collected);
	filecontent.replace("$info", "gernerated with readesm");
	return filecontent;
}