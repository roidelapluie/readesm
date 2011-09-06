#include "Reporter.h"
#include <QTextCodec>

void Reporter::setTitle(const QString& newTitle){
	title = newTitle;
}

void Reporter::tagValuePair(const QString& tag, int value){
	tagValuePair(tag, QString("%1").arg(value));
}

Reporter::Reporter() : collected(), collector(&collected)
{
	collector.setCodec(QTextCodec::codecForName("UTF-8"));
}

void Reporter::flush(){
	collector.flush();
}
