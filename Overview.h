/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef OVERVIEW_H
#define OVERVIEW_H OVERVIEW_H
#include <string>
#include "vublock.h"
#include "formatStrings.h"
#ifndef HAVE_NO_CRYPTO
#include "crypto.h"
#endif
#include "readTypes.h"

///See page 160 of l207.pdf
class Overview : public vublock {
	public:
	static const int TREP = 0x1;
	virtual string name() const{
		return "Overview";
	}
	Overview(iter nstart) : vublock(nstart) { Init(); }
	int size() const{
		int num_locks = block_start[491];
		int num_ctrls = block_start[492 + num_locks * 98];
		return 2 + 491 + 2 + 128 + num_locks * 98 + num_ctrls * 31;
	}
	void CompleteReport(reporter& report) const{
		runningIndex = 388;
		report("VehicleIdentificationNumber",fixedString(17));
		report("VehicleRegistrationNation",  nationNumeric(IntByte()));
		report("VehicleRegistrationNumber", fixedString(14));

		report("CurrentDateTime",	readDate().str());
		report("minDownloadableTime",	readDate().str());
		report("maxDownloadableTime",	readDate().str());
		report("CardSlotsStatus",	IntByte());
		report("downloadingTime",	readDate().str());
		report("fullCardNumber",	fixedString(18));
		report("companyOrWorkshopName",	fixedString(36));
		for(reporter::subblock b = report.newsub("VuCompanyLocksRecord", IntByte()); b(); ++b){
			report("lockInTime",	readDate().str());
			report("lockOutTime",	readDate().str());	
			report("companyName",	fixedString(36));
			report("companyAddress",fixedString(36));
			report("companyCardNumber",	fixedString(18));
		}
		for(reporter::subblock b = report.newsub("VuControlActivityRecord", IntByte()); b(); ++b){
			report("controlType",	formatControlType(IntByte()));
			report("controlTime",	readDate().str());
			report("controlCardNumber",	fixedString(18));
			report("downloadPeriodBeginTime",readDate().str());	
			report("downloadPeriodEndTime",readDate().str());
		}
	}
	virtual void reportstuff(esmfilehead& esm){
#ifndef HAVE_NO_CRYPTO
		if(esm.CAcert) std::cerr << "Reassigning CAcert\n";
		esm.CAcert = verifiedcert::ptr(new verifiedcert(start + 2));
		if(esm.devicecert) std::cerr << "Reassigning devicecert\n";
		esm.devicecert = verifiedcert::ptr(new verifiedcert(start + 2 + 194));
#endif
		esm.title = ::fixedString(start + 390 + 18, 14);

	}
	int nonhashedbytes() const{ return 2*194;}
	void BriefReport(reporter& report) const{
		CompleteReport(report);
	}
};
#endif
