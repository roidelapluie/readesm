#include "PictureGenerator.h"

PictureGenerator::PictureGenerator() : collected(), collector(&collected) {}

QString PictureGenerator::toString() const{
	return collected;
}

void PictureGenerator::reset(){
	collected = "";
}
