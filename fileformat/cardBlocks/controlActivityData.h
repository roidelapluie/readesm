#ifndef CONTROLACTIVITYDATA_H
#define CONTROLACTIVITYDATA_H

#include "cardBlock.h"
#include "../dataTypes/fullCardNumber.h"
#include "../dataTypes/vehicleRegistration.h"

class controlActivityData : public cardBlock {
	Q_DECLARE_TR_FUNCTIONS(controlActivityData)
	public:
	static const int Type = 0x0508;
	int controlType;
	Time controlTime;
	fullCardNumber controlCardNumber;
	vehicleRegistration controlVehicleRegistration;
	Time controlDownloadPeriodBegin, controlDownloadPeriodEnd;
	virtual QString name() const {
		return tr("Control Activity Data");
	}
	controlActivityData(constDataPointer filewalker) :
		cardBlock(filewalker), 
		controlType(start[5]), 
		controlTime(BEInt32(start + 6)),
		controlCardNumber(start + 10),
		controlVehicleRegistration(start + 28),
		controlDownloadPeriodBegin(BEInt32(start + 43)),
		controlDownloadPeriodEnd(BEInt32(start + 47))
	{
	}
	virtual void printOn(reporter& o) const {
		o(tr("controlType"), formatStrings::controlType(controlType));
		o(tr("controlTime"), controlTime.str());
		o << controlCardNumber;
		o << controlVehicleRegistration;
		o(tr("controlDownloadPeriodBegin"), controlDownloadPeriodBegin.str());
		o(tr("controlDownloadPeriodEnd"), controlDownloadPeriodEnd.str());
	}
};

#endif
