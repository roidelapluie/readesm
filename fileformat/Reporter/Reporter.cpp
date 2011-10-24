#include "Reporter.h"
#include "../Block.h"
#include <QtCore/QDebug>
#include <QTextCodec>

void Reporter::setTitle(const QString& newTitle){
	title = newTitle;
}

void Reporter::tagValuePair(const QString& tag, int value){
	tagValuePair(tag, QString("%1").arg(value));
}

Reporter::Reporter() : nestLevel(0), collected(), collector(&collected)
{
	collector.setCodec(QTextCodec::codecForName("UTF-8"));
}

void Reporter::writeBlock(const Block& value, const QString& tag){
	++nestLevel;
	if(tag == "" && value.title() != "") subBlock(value, value.title());
	else subBlock(value, tag);
	--nestLevel;
}
