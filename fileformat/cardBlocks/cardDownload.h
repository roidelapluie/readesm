#ifndef CARDDOWNLOAD_H
#define CARDDOWNLOAD_H

#include "cardBlock.h"

class cardDownload : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(cardDownload)
	public:
	static const int Type = 0x050e;
	QString name() const {
		return tr("Card Download");
	}
	Time LastCardDownload;
	cardDownload(constDataPointer filewalker) :
		cardBlock(filewalker), 
		LastCardDownload(Time(BEInt32(filewalker + 5))) 
	{
	}
	virtual void printOn(reporter& o) const {
		o(tr("LastCardDownload"), LastCardDownload.str());
	}
};

#endif
