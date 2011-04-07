#ifndef APPLICATIONIDENTIFICATION_H
#define APPLICATIONIDENTIFICATION_H

#include "cardBlock.h"
#include "../formatStrings.h"

class applicationIdentification : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(applicationIdentification)
	public:
	static const int Type = 0x0501;
	QString name() const {
		return tr("Application Identification");
	}
	int typeOfTachographCardId;
	int cardStructureVersion;
	int noOfEventsPerType;
	int noOfFaultsPerType;
	int activityStructureLength;
	int noOfCardVehicleRecords;
	int noOfCardPlaceRecords;
	
	applicationIdentification(constDataPointer filewalker) :
		cardBlock(filewalker), 
		typeOfTachographCardId(start[5]),
		cardStructureVersion(BEInt16(start + 6)),
		noOfEventsPerType(start[8]),
		noOfFaultsPerType(start[9]),
		activityStructureLength(BEInt16(start + 10)),
		noOfCardVehicleRecords(BEInt16(start + 12)),
		noOfCardPlaceRecords(start[5])
	{
	}
	virtual void printOn(reporter& o) const {
		o(tr("typeOfTachographCardId"), formatStrings::equipmentType(typeOfTachographCardId));
		o(tr("cardStructureVersion"), hex(cardStructureVersion, 4));
		o(tr("noOfEventsPerType"), noOfEventsPerType);
		o(tr("noOfFaultsPerType"), noOfFaultsPerType);
		o(tr("activityStructureLength"), activityStructureLength);
		o(tr("noOfCardVehicleRecords"), noOfCardVehicleRecords);
		o(tr("noOfCardPlaceRecords"), noOfCardPlaceRecords);
	}
};

#endif
