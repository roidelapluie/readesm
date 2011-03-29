#ifndef FAULTSDATA_H
#define FAULTSDATA_H

#include "eventsData.h"

class faultsData : public eventsData {
	Q_DECLARE_TR_FUNCTIONS(faultsData)
	public:
	faultsData(constDataPointer filewalker) :
		eventsData(filewalker) 
	{
	}
	virtual QString name() const {
		return tr("Faults Data");
	}
	static const int Type = 0x0503;
};

#endif
