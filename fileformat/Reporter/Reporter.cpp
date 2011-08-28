#include "Reporter.h"

void Reporter::setTitle(const QString& newTitle){
	title = newTitle;
}

void Reporter::tagValuePair(const QString& tag, int value){
	tagValuePair(tag, QString("%1").arg(value));
}

Reporter::Reporter() : collected(), collector(&collected)
{
}

void Reporter::flush(){
	collector.flush();
}