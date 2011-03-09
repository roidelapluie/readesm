/* Copyright 2009 Andreas Gölzer

 This file is part of readESM.

 readESM is free software: you can redistribute it and/or modify it under the
 terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 readESM is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 readESM.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef OVERVIEW_H
#define OVERVIEW_H OVERVIEW_H

#include "config.h"
#include "../constDataPointer.h"

#ifndef HAVE_NO_CRYPTO
#include "../crypto.h"
#endif

#include "../formatStrings.h"
#include "../readTypes.h"
#include "vuBlock.h"

#include <QtCore/QDebug>
#include <QtCore/QString>

///See page 160 of l207.pdf
class vuOverview : public vuBlock {
	Q_DECLARE_TR_FUNCTIONS(vuOverview)
	public:
	static const int TREP = 0x1;
	virtual QString name() const {
		return tr("Overview");
	}
	vuOverview(constDataPointer nstart) :
		vuBlock(nstart) {
		Init();
	}
	int size() const {
		int num_locks = block_start[491];
		int num_ctrls = block_start[492 + num_locks * 98];
		return 2 + 491 + 2 + 128 + num_locks * 98 + num_ctrls * 31;
	}
	void CompleteReport(reporter& report) const {
		runningIndex = 388;
		report(tr("VehicleIdentificationNumber"), fixedString(17));
		report(tr("VehicleRegistrationNation"), formatStrings::nationNumeric(IntByte()));
		report(tr("VehicleRegistrationNumber"), fixedString(14));

		report(tr("CurrentDateTime"), readDate().str());
		report(tr("minDownloadableTime"), readDate().str());
		report(tr("maxDownloadableTime"), readDate().str());
		report(tr("CardSlotsStatus"), IntByte());
		report(tr("downloadingTime"), readDate().str());
		report(tr("fullCardNumber"), fixedString(18));
		report(tr("companyOrWorkshopName"), fixedString(36));
		for(reporter::subblock b = report.newsub("VuCompanyLocksRecord",
				IntByte()); b(); ++b) {
			report(tr("lockInTime"), readDate().str());
			report(tr("lockOutTime"), readDate().str());
			report(tr("companyName"), fixedString(36));
			report(tr("companyAddress"), fixedString(36));
			report(tr("companyCardNumber"), fixedString(18));
		}
		for(reporter::subblock b = report.newsub("VuControlActivityRecord",
				IntByte()); b(); ++b) {
			report(tr("controlType"), formatStrings::controlType(IntByte()));
			report(tr("controlTime"), readDate().str());
			report(tr("controlCardNumber"), fixedString(18));
			report(tr("downloadPeriodBeginTime"), readDate().str());
			report(tr("downloadPeriodEndTime"), readDate().str());
		}
	}
	virtual void reportstuff(esmfilehead& esm) {
#ifndef HAVE_NO_CRYPTO
		if(esm.CAcert) qDebug() << "Reassigning CAcert\n";
		esm.CAcert = verifiedcert::ptr(new verifiedcert(start + 2));
		if(esm.devicecert) qDebug() << "Reassigning devicecert\n";
		esm.devicecert = verifiedcert::ptr(new verifiedcert(start + 2 + 194));
#endif
		esm.title = ::fixedString(start + 390 + 18, 14);

	}
	int nonhashedbytes() const {
		return 2* 194 ;}
		void BriefReport(reporter& report) const {
			CompleteReport(report);
		}
	};
#endif
