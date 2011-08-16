#include "blockFactory.h"

#include "CardBlocks/cardBlockFactory.h"
#include "VuBlocks/vuBlockFactory.h"

#include <QtCore/QDebug>

QSharedPointer<Block> blockFactory(const constDataPointer& start) {
	qDebug() << "Creating block at" << start.offset << "bytes left: " << start.bytesLeft();
	if(start[0] == 0x76) return vuBlockFactory(start);
	else return cardBlockFactory(start);
}
