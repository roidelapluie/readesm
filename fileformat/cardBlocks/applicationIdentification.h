#ifndef APPLICATIONIDENTIFICATION_H
#define APPLICATIONIDENTIFICATION_H

#include "cardBlock.h"

class applicationIdentification : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(applicationIdentification)
	public:
	static const int Type = 0x0501;
	QString name() const {
		return tr("Application Identification");
	}
	int typeOfTachographCardId, cardStructureVersion;
	applicationIdentification(constDataPointer filewalker) :
		cardBlock(filewalker), 
		typeOfTachographCardId(start[5]),
		cardStructureVersion(BEInt16(start + 6)) 
	{
	}
	virtual void printOn(reporter& o) const {
		o(tr("typeOfTachographCardId"), formatStrings::equipmentType(typeOfTachographCardId));
		o(tr("cardStructureVersion"), hex(cardStructureVersion, 4));
		//TODO: read noOfCompanyActivityRecords
	}
};

#endif
