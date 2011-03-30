#include "cardBlock.h"

#include "cardDownload.h"
#include "drivingLicenseInfo.h"
#include "currentUsage.h"
#include "driverActivityData.h"
#include "identification.h"
#include "specificConditions.h"
#include "faultsData.h"
#include "eventsData.h"
#include "vehiclesUsed.h"
#include "cardCertificate.h"
#include "caCertificate.h"
#include "applicationIdentification.h"
#include "controlActivityData.h"
#include "places.h"
#include "cardIccIdentification.h"
#include "cardChipIdentification.h"

//Driver Card Codes and structure are on p. 119 in l207.pdf
cardBlock::ptr cardBlock::Factory(constDataPointer filewalker) {
	if(filewalker[2] == 1) qFatal("stray signature");
	typedef cardBlock::ptr p;
	switch(cardBlock::getType(filewalker)) {
		case cardDownload::Type:
			return p(new cardDownload(filewalker));
		case drivingLicenseInfo::Type:
			return p(new drivingLicenseInfo(filewalker));
		case currentUsage::Type:
			return p(new currentUsage(filewalker));
		case driverActivityData::Type:
			return p(new driverActivityData(filewalker));
		case identification::Type:
			return p(new identification(filewalker));
		case specificConditions::Type:
			return p(new specificConditions(filewalker));
		case faultsData::Type:
			return p(new faultsData(filewalker));
		case eventsData::Type:
			return p(new eventsData(filewalker));
		case vehiclesUsed::Type:
			return p(new vehiclesUsed(filewalker));
		case cardCertificate::Type:
			return p(new cardCertificate(filewalker));
		case caCertificate::Type:
			return p(new caCertificate(filewalker));
		case applicationIdentification::Type:
			return p(new applicationIdentification(filewalker));
		case controlActivityData::Type:
			return p(new controlActivityData(filewalker));
		case places::Type:
			return p(new places(filewalker));
		case cardIccIdentification::Type:
			return p(new cardIccIdentification(filewalker));
		case cardChipIdentification::Type:
			return p(new cardChipIdentification(filewalker));
		
		default:
			return p(new cardBlock(filewalker));
	}
}