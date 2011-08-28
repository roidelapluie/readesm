#include "Block.h"

#include "config.h"

Block::Block(const DataPointer& filewalker) :
	start(filewalker)
{
}

Reporter& operator<<(Reporter& report, const Block& b){
	b.printOn(report);
	return report;
}