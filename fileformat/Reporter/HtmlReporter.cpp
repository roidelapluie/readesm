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
		if(value.toString() != ""){
			if(tag == "") collector << toggleAbleBlocks(value.toString().replace("&","&amp;"), false);
			else collector << toggleAbleBlocks(QString("%1: <b>%2</b>").arg(lTag, value.toString().replace("&","&amp;")), false);
			*this << value;
			collector << "</ul></li>";
		} else {
			if(tag != "") collector << "\n<li>" << lTag << ": <ul>";
			else collector << "\n<li><ul>";
			*this << value;
			collector << "</ul></li>";
		}
	}
}

QString HtmlReporter::toggleAbleBlocks(const QString& title, bool showByDefault){
	++toggleNumber;
	return QString("\n<li>%1 (<a href='#link%2' name='link%2' onclick='return toggle_visibility(%2);' id='link%2'>%3</a>)<ul name='toggled%2' style='%4' id='toggled%2'>")
			.arg(title)
			.arg(toggleNumber)
			.arg(showByDefault ? tr("hide") : tr("show"))
			.arg(showByDefault ? "" : "display:none");
}

QByteArray HtmlReporter::toQByteArray() const{
	collector.flush();
	linkCollector.flush();
	if(collected.size() == 0) return QByteArray();
	QByteArray filecontent = loadFile(":/template.html");
	filecontent.replace("$title", title.toUtf8());
	filecontent.replace("$content", collected);
	filecontent.replace("$show", tr("show").toUtf8());
	filecontent.replace("$hide", tr("hide").toUtf8());
	filecontent.replace("$links", linkCollected + "</ul>");
	filecontent.replace("$version", "readesm " VERSION " (" VERSION_DATE ")");
	return filecontent;
}

bool HtmlReporter::allowSvg() const{
	return true;
}

void HtmlReporter::arrayStart(int count, const QString& title, bool defaultShown){
	if(count > 0){
		collector << toggleAbleBlocks(QString("%1 %2").arg(count).arg(title), defaultShown);
	} else collector << "<li>" << tr("No %1.").arg(title) << "</li>";
}
void HtmlReporter::arrayEnd(int count){
	if(count > 0) collector << "</ul></li>";
}
