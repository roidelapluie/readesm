#include "blockFactory.h"

#include "CardBlocks/cardBlockFactory.h"
#include "VuBlocks/vuBlockFactory.h"

#include <QtCore/QDebug>

QSharedPointer<TopLevelBlock> blockFactory(const DataPointer& start) {
	//qDebug() << "attempting to create block, bytes left: " << start.bytesLeft() << hexByte(readBigEndianInt1(start)) << hexByte(readBigEndianInt2(start));
	QSharedPointer<TopLevelBlock> rv;
	if(start[0] == 0x76) rv = vuBlockFactory(start);
	else rv = cardBlockFactory(start);
	//qDebug() << "Created block, named " << rv->name() << " at" << start.offset << "bytes left: " << start.bytesLeft();
	return rv;
}

