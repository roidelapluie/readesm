#ifndef CARDBLOCK_H
#define CARDBLOCK_H

#include "../block.h"
#include "../formatStrings.h"

class cardBlock : public block {
	public:
	cardBlock(constDataPointer filewalker) :
		block(filewalker) 
	{
		datasize = BEInt16(start + 3);
		qDebug() << "size" << datasize << "type" << hex(type) << "Left: " << filewalker.bytesLeft();
		filewalker = start + 5 + datasize;
		if(filewalker.bytesLeft() >= 5 + 128 && getType(filewalker) == type && filewalker[2] == 1) {
			signature = filewalker + 5;
			hassignature = true;
		}
	}
	int size() const {
		return datasize + 5 + (hassignature ? 5 + 128 : 0);
	}
	static ptr Factory(constDataPointer filewalker);
#ifndef HAVE_NO_CRYPTO
	virtual bool checksig(const rsa& key) {
		if(hassignature) validsignature = CheckSignature(start + 5, datasize,
				signature, 128, key);
		return validsignature;
	}
#endif
	protected:
	int datasize;

};

#endif
