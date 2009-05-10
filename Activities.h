/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef ACTIVITIES_H
#define ACTIVITIES_H ACTIVITIES_H
#include <string>
#include <vector>
#include "reporter.h"
#include "vublock.h"
#include "Activity.h"
#include "formatStrings.h"


///See page 161 of l207.pdf
class Activities : public vublock {
	struct CardInOut {
		string Name;
		Time InTime, OutTime;
		int OdometerIn, OdometerOut;
	};
	DailyActivity DayAct;
	int driven;
	std::vector<CardInOut> CardInOuts;
	Time day;
	Activity readActivity(int start) const{
		return Activity(&block_start[start]);
	}
	Activity readActivity() const{
		runningIndex += 2;
		return Activity(&block_start[runningIndex - 2]);
	}
	public:
	virtual string name() const{
		return "Activities for " + readDate(0).datestr();
	}
	static const int TREP = 0x2;
	Activities(iter nstart) : vublock(nstart), driven(0), day(BEInt32(start + 2)) {
		Init();
		int index = 7 + 129*Int16(7) + 2;
		DayAct = DailyActivity(start + 2, start + index + 4, Int16(index));
		int num_crblocks = Int16(7);
		for(int j = 0; j < num_crblocks; ++j){
			int sb_start = 9 + j*129;
			CardInOut t;
			t.Name = fixedString(sb_start + 36,36) + fixedString(sb_start,36);
			t.InTime = readDate(sb_start + 94);
			t.OdometerIn = Odometer(sb_start + 98);
			t.OutTime = readDate(sb_start + 102);
			t.OdometerOut = Odometer(sb_start + 106);
			CardInOuts.push_back(t);
			if(t.InTime.timestamp >= day.timestamp && t.InTime.timestamp < day.timestamp + 86400) driven += t.OdometerOut - t.OdometerIn;
		}
	}
	int size() const{
		int index = 7 + 129*Int16(7) + 2;
		index += 2*Int16(index) + 2;
		index += 28*IntByte(index) + 1;
		index += 5*Int16(index) + 2;
		return 2 + index + 128;
	}
	void CompleteReport(reporter& report) const{
		runningIndex = 0;
		report("TimeReal",readDate().str());
		report("Odometer",Odometer());
		for(reporter::subblock b = report.newsub("VuCardIWRecord", Int16()); b(); ++b){
			report("cardHolderSurname", fixedString(36));
			report("cardHolderFirstNames", fixedString(36));
			report("fullCardNumber", fixedString(18));
			report("cardExpiryDate", readDate().str());
			report("cardInsertionTime", readDate().str());
			report("vehicleOdometerValueAtInsertion", Odometer());
			report("cardSlotNumber", IntByte());
			report("cardWithdrawalTime", readDate().str());
			report("vehicleOdometerValueAtWithdrawal", Odometer());
			report("previousVehicleRegistrationNation", nationNumeric(IntByte()));
			report("previousVehicleRegistrationNumber", fixedString(14));
			report("previousCardWithdrawalTime", readDate().str());
			report("manualInputFlag", IntByte());
		}

		for(reporter::subblock b = report.newsub("activityChangeInfos", Int16()); b(); ++b){
			report("activityChangeInfo", readActivity().str());
		}
		
		for(reporter::subblock b = report.newsub("Regions", IntByte()); b(); ++b){
			report("fullCardNumber", fixedString(18));
			report("entryTime", readDate().str());
			report("entryTypeDailyWorkPeriod", formatDailyWorkPeriod(IntByte()));
			report("dailyWorkPeriodCountry", nationNumeric(IntByte()));
			report("dailyWorkPeriodRegion", IntByte());
			report("vehicleOdometerValue", Odometer());
		}
		for(reporter::subblock b = report.newsub("SpecificConditionRecord", Int16()); b(); ++b){
			report("entryTime", readDate().str());
			report("specificConditionType", IntByte());
		}
	}
	void BriefReport(reporter& report) const{
		for(unsigned int j = 0; j < CardInOuts.size() ; ++j){
			const CardInOut& t = CardInOuts[j];
			report("Driver",t.Name);
			report("Work period", (t.OutTime - t.InTime).str());
			report("Driven km",(t.OdometerOut - t.OdometerIn));
		}
		report << DayAct;
		//report("Driven km",driven);
	}
	virtual void reportstuff(esmfilehead& esm){
		esm.reportDayStatistics(Time(BEInt32(start + 2)),driven,DayAct.driventime);
	}
};

#endif
