#ifndef CURRENTUSAGE_H
#define CURRENTUSAGE_H

#include "cardBlock.h"

#include "../dataTypes/vehicleRegistration.h"

//contrary to specification, the default value for the vehicle registration here
class currentUsage : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(currentUsage)
	public:
	static const int Type = 0x0507;
	QString name() const {
		return tr("Current Usage");
	}
	Time sessionOpenTime;
	vehicleRegistration reg;

	currentUsage(constDataPointer filewalker) :
		cardBlock(filewalker), 
		sessionOpenTime(BEInt32(start + 5)),
		reg(start + 5 + 4)
	{
	}

	virtual void printOn(reporter& o) const {
		o(tr("sessionOpenTime"), sessionOpenTime.str());
		//qDebug() << "usage..." << formatStrings::nationNumeric(reg.Nation) << reg.Number.length() << "regnum:" << reg.Number << "enddd";
		o << reg;
		//qDebug() << "cuse " << start.offset;
	}
};

#endif
