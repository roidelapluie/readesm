#include "PictureGenerator.h"

#include <QtCore/QTextCodec>

PictureGenerator::PictureGenerator() : collected(), collector(&collected) {
	collector.setCodec(QTextCodec::codecForName("UTF-8"));
}

QString PictureGenerator::toString() const{
	return collected;
}

void PictureGenerator::reset(){
	collected = "";
}
