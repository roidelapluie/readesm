#include "block.h"

#include "cardblocks.h"
#include "vuBlocks/vuBlock.h"
#include "vuBlocks/vuFactory.h"

block::ptr block::Factory(constDataPointer start) {
	if(start[0] == 0x76) return block::ptr(vuFactory(start));
	else return cardBlock::Factory(start);
}
