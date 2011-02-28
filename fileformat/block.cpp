#include "block.h"

#include "vuBlocks/vuFactory.h"
#include "cardblocks.h"

block::ptr block::Factory(constDataPointer start) {
	if(start[0] == 0x76) return block::ptr(vuFactory(start));
	else return tlvblock::Factory(start);
}