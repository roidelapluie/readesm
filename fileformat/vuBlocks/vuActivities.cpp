#include "vuActivities.h"

#include "reporter.h"
#include "vuBlock.h"
#include "Activity.h"
#include "formatStrings.h"
#include "constDataPointer.h"

#include <QtCore/QString>

#include <vector>

Activity vuActivities::readActivity(int start) const {
	return Activity(&block_start[start]);
}
Activity vuActivities::readActivity() const {
	runningIndex += 2;
	return Activity(&block_start[runningIndex - 2]);
}

QString vuActivities::name() const {
	return tr("Activities for") + " " + readDate(0).datestr();
}

vuActivities::vuActivities(constDataPointer nstart) :
	vuBlock(nstart), 
	driven(0), 
	day(BEInt32(start + 2)) 
{
	Init();
	int index = 7 + 129* Int16 (7) + 2;
	DayAct = dailyActivity(start + 2, start + index + 4, Int16(index));
	int num_crblocks = Int16(7);
	for(int j = 0; j < num_crblocks; ++j) {
		int sb_start = 9 + j * 129;
		CardInOut t;
		t.Name = fixedString(sb_start + 36, 36) + " " + fixedString(
				sb_start, 36);
		t.InTime = readDate(sb_start + 94);
		t.OdometerIn = Odometer(sb_start + 98);
		t.OutTime = readDate(sb_start + 102);
		t.OdometerOut = Odometer(sb_start + 106);
		CardInOuts.push_back(t);
		if(t.InTime.timestamp >= day.timestamp && t.InTime.timestamp
				< day.timestamp + 86400) driven += t.OdometerOut
				- t.OdometerIn;
	}
}

int vuActivities::size() const {
	int index = 7 + 129* Int16 (7) + 2;
	index += 2* Int16 (index) + 2;
	index += 28* IntByte (index) + 1;
	index += 5* Int16 (index) + 2;
	return 2 + index + 128;
}

void vuActivities::CompleteReport(reporter& report) const {
	runningIndex = 0;
	report(tr("TimeReal"), readDate().str());
	report(tr("Odometer"), Odometer());
	for(reporter::subblock b = report.newsub(tr("VuCardIWRecord"), Int16()); b(); ++b) {
		report(tr("cardHolderSurname"), fixedString(36));
		report(tr("cardHolderFirstNames"), fixedString(36));
		report(tr("fullCardNumber"), fixedString(18));
		report(tr("cardExpiryDate"), readDate().str());
		report(tr("cardInsertionTime"), readDate().str());
		report(tr("vehicleOdometerValueAtInsertion"), Odometer());
		report(tr("cardSlotNumber"), IntByte());
		report(tr("cardWithdrawalTime"), readDate().str());
		report(tr("vehicleOdometerValueAtWithdrawal"), Odometer());
		report(tr("previousVehicleRegistrationNation"),
				formatStrings::nationNumeric(IntByte()));
		report(tr("previousVehicleRegistrationNumber"), fixedString(14));
		report(tr("previousCardWithdrawalTime"), readDate().str());
		report(tr("manualInputFlag"), IntByte());
	}

	for(reporter::subblock b =
			report.newsub(tr("activityChangeInfos"), Int16()); b(); ++b) {
		report(tr("activityChangeInfo"), readActivity().str());
	}

	for(reporter::subblock b = report.newsub("Regions", IntByte()); b(); ++b) {
		report(tr("fullCardNumber"), fixedString(18));
		report(tr("entryTime"), readDate().str());
		report(tr("entryTypeDailyWorkPeriod"), formatStrings::dailyWorkPeriod(IntByte()));
		report(tr("dailyWorkPeriodCountry"), formatStrings::nationNumeric(IntByte()));
		report(tr("dailyWorkPeriodRegion"), IntByte());
		report(tr("vehicleOdometerValue"), Odometer());
	}
	for(reporter::subblock b = report.newsub(tr("SpecificConditionRecord"),
			Int16()); b(); ++b) {
		report(tr("entryTime"), readDate().str());
		report(tr("specificConditionType"), IntByte());
	}
}
void vuActivities::BriefReport(reporter& report) const {
	for(unsigned int j = 0; j < CardInOuts.size(); ++j) {
		const CardInOut& t = CardInOuts[j];
		report(tr("Driver"), t.Name);
		report(tr("Work period"), (t.OutTime - t.InTime).str());
		report(tr("Driven distance"), stringify(t.OdometerOut - t.OdometerIn)
				+ " km");
	}
	report << DayAct;
	//report(tr("Driven km"),driven);
}

void vuActivities::reportstuff(esmfilehead& esm) {
	esm.reportDayStatistics(Time(BEInt32(start + 2)), driven,
			DayAct.driventime);
}

