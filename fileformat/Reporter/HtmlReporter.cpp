#include "HtmlReporter.h"

#include "../loadFile.h"
#include "../DataTypes/DataType.h"
#include "../TopLevelBlock.h"
#include "config.h"
#include <QTextCodec>

HtmlReporter::HtmlReporter() : Reporter(), 
	linkNumber(0),
	linkCollected(),
	linkCollector(&linkCollected)
{
	linkCollector.setCodec(QTextCodec::codecForName("UTF-8"));
	linkCollector << "<h3>" << tr("Content") << "</h3><ul>";
}

void HtmlReporter::tagValuePair(const QString& tag, const QString& value){
	QString copy(value);
	copy.replace("&","&amp;");
	collector << "\n<li>" << tag << ": <b>" << copy << "</b></li>";
}

void HtmlReporter::subBlock(const Block& value, const QString& tag){
	QString lTag = tag;
	if(nestLevel == 1){
		++linkNumber;
		linkCollector << "<li><a href='#" << linkNumber << "'>" << tag << "</a></li>";
		lTag = QString("<a name='%1'>%2</a>").arg(linkNumber).arg(tag); 
	}
	if(nestLevel == 1){
		collector << "\n<h3>" << lTag << "</h3>";
		collector << "<ul>";
		*this << value;
		collector << "</ul>";
	} else {
		if(tag != "") collector << "\n<li>" << lTag << ": <ul>";
		else collector << "\n<li><ul>";
		*this << value;
		collector << "</ul></li>";
	}
}


QByteArray HtmlReporter::toQByteArray() const{
	collector.flush();
	linkCollector.flush();
	QByteArray filecontent = loadFile(":/template.html");
	filecontent.replace("$title", title.toUtf8());
	filecontent.replace("$content", collected);
	filecontent.replace("$links", linkCollected + "</ul>");
	filecontent.replace("$version", "readesm " VERSION " (" VERSION_DATE ")");
	return filecontent;
}

bool HtmlReporter::allowSvg() const{
	return true;
}

void HtmlReporter::arrayStart(int count, const QString& title, bool defaultShown){
	++toggleNumber;
	if(count > 0){
		collector << QString("\n<li>%1 %2 (<a href='#link%3' name='link%3' onclick='toggle_visibility(%3);' id='link%3'>%4</a>)<ul name='toggled%3' style='%5' id='toggled%3'>")
			.arg(count)
			.arg(title)
			.arg(toggleNumber)
			.arg(defaultShown ? tr("hide") : tr("show"))
			.arg(defaultShown ? "" : "display:none");
	} else collector << "<li>" << tr("No %1.").arg(title) << "</li>";
}
void HtmlReporter::arrayEnd(int count){
	if(count > 0) collector << "</ul></li>";
}
