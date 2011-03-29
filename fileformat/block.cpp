#include "block.h"

#include "cardBlocks/cardBlock.h"
#include "vuBlocks/vuBlock.h"

block::ptr block::Factory(constDataPointer start) {
	//qDebug() << "Creating block at" << start.offset << "bytes left: " << start.bytesLeft();
	if(start[0] == 0x76) return block::ptr(vuBlock::Factory(start));
	else return cardBlock::Factory(start);
}
