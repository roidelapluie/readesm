#include "block.h"

#include "cardblocks.h"
#include "vuBlocks/vuBlock.h"
#include "vuBlocks/vuFactory.h"
#include "hexBlock.h"

block::ptr block::Factory(constDataPointer start) {
	qDebug() << "Creating block at" << start.offset << "bytes left: " << start.bytesLeft();
	if(start[0] == 0x76){
		block::ptr temp(vuFactory(start));
		if(temp.isNull()) return block::ptr(new hexBlock(start));
		return temp;
	} 
	else return cardBlock::Factory(start);
}
