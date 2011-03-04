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

#ifndef CARDBLOCKS_H
#define CARDBLOCKS_H

#include <QDebug>

#include "Activity.h"
#include "esmfilehead.h"
#ifndef HAVE_NO_CRYPTO
#include "crypto.h"
#endif
#include "block.h"
#include "overtime.h"
#include "readTypes.h"
#include "formatStrings.h"
#include "constDataPointer.h"

class tlvblock : public block {
	public:
	tlvblock(constDataPointer filewalker) :
		block(filewalker) 
	{
		datasize = BEInt16(start + 3);
		qDebug() << "size" << datasize;
		filewalker = start + 5 + datasize;
		if(getType(filewalker) == type && filewalker[2] == 1) {
			signature = filewalker + 5;
			hassignature = true;
		}
	}
	int size() const {
		return datasize + 5 + (hassignature ? 5 + 128 : 0);
	}
	static ptr Factory(constDataPointer filewalker);
#ifndef HAVE_NO_CRYPTO
	virtual bool checksig(const rsa& key) {
		if(hassignature) validsignature = CheckSignature(start + 5, datasize,
				signature, 128, key);
		return validsignature;
	}
#endif
	protected:
	int datasize;

};
class Driving_License_Info : public tlvblock {
	public:
	QString drivingLicenseIssuingAuthorithy;
	static const int Type = 0x0521;
	int drivingLicenseIssuingNation;
	QString drivingLicenseNumber;
	QString name() const {
		return tr("Driving License Info");
	}
	Driving_License_Info(constDataPointer filewalker) :
		tlvblock(filewalker), drivingLicenseIssuingAuthorithy(fixedString(start
				+ 5, 36)), drivingLicenseIssuingNation(start[5 + 36]),
				drivingLicenseNumber(fixedString(start + 5 + 36 + 1, 16)) {
	}

	virtual void printOn(reporter& o) const {
		o(tr("drivingLicenceIssuingAuthorithy"), drivingLicenseIssuingAuthorithy);
		o(tr("drivingLicenceIssuingNation"), formatStrings::nationNumeric(drivingLicenseIssuingNation));
		o(tr("drivingLicenceNumber"), drivingLicenseNumber);
	}
};

class Card_Download : public tlvblock {
	public:
	static const int Type = 0x050e;
	QString name() const {
		return tr("Card Download");
	}
	Time LastCardDownload;
	Card_Download(constDataPointer filewalker) :
		tlvblock(filewalker), LastCardDownload(Time(BEInt32(filewalker + 5))) {
	}
	virtual void printOn(reporter& o) const {
		o(tr("LastCardDownload"), LastCardDownload.str());
	}
};

class Application_Identification : public tlvblock {
	public:
	static const int Type = 0x0501;
	QString name() const {
		return tr("Application Identification");
	}
	int typeOfTachographCardId, cardStructureVersion;
	Application_Identification(constDataPointer filewalker) :
		tlvblock(filewalker), typeOfTachographCardId(start[5]),
				cardStructureVersion(BEInt16(start + 6)) {
	}
	virtual void printOn(reporter& o) const {
		o(tr("typeOfTachographCardId"), formatStrings::equipmentType(typeOfTachographCardId));
		o(tr("cardStructureVersion"), hex(cardStructureVersion, 4));
		//TODO: read noOfCompanyActivityRecords
	}
};

class Card_Certificate : public tlvblock {
	public:
	static const int Type = 0xc100;
	virtual QString name() const {
		return tr("Card Certificate");
	}
	Card_Certificate(constDataPointer filewalker) :
		tlvblock(filewalker) {
	}
	virtual void reportstuff(esmfilehead& esm) {
#ifndef HAVE_NO_CRYPTO
		if(esm.devicecert) qDebug() << "Reassigning cert";
		esm.devicecert = verifiedcert::ptr(new verifiedcert(start + 5));
#endif
	}
};

class CA_Certificate : public Card_Certificate {
	public:
	static const int Type = 0xc108;
	virtual QString name() const {
		return tr("CA Certificate");
	}
	CA_Certificate(constDataPointer filewalker) :
		Card_Certificate(filewalker) {
	}
	virtual void reportstuff(esmfilehead& esm) {
#ifndef HAVE_NO_CRYPTO
		if(esm.CAcert) qDebug() << "Reassigning cert\n";
		esm.CAcert = verifiedcert::ptr(new verifiedcert(start + 5));
#endif
	}
};

class Specific_Conditions : public tlvblock {
	struct Specific_Condition {
		Time time;
		int condition;
		Specific_Condition(Time time_, int condition_) :
			time(time_), condition(condition_) {
		}
		friend reporter& operator<<(reporter& report, Specific_Condition scond) {
			report(scond.time.str(), formatStrings::specificCondition(scond.condition));
			return report;
		}
	};
	public:
	typedef std::vector<Specific_Condition> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0522;
	QString name() const {
		return tr("Specific Conditions");
	}
	Specific_Conditions(constDataPointer filewalker) :
		tlvblock(filewalker) {
		for(constDataPointer i = start + 5; i < start + 5 + datasize; i += 5) {
			int time = BEInt32(i), cond = i[4];
			if(time != 0 || cond != 0) sub.push_back(Specific_Condition(time,
					cond));
		}
	}
	virtual void printOn(reporter& o) const {
		for(subiter i = sub.begin(); i < sub.end(); ++i)
			o << *i;
	}
};

struct vehicleRegistration {
	Q_DECLARE_TR_FUNCTIONS(vehicleRegistration)
public:
	int Nation;
	QString Number;
	vehicleRegistration(constDataPointer start) :
		Nation(start[0]), Number(fixedString(start + 1, 14)) {
	}
	friend reporter& operator<<(reporter& report, vehicleRegistration sub) {
		if(report.verbose) report(tr("vehicleRegistrationNation"), formatStrings::nationNumeric(sub.Nation));
		report(tr("vehicleRegistrationNumber"), sub.Number);
		return report;
	}
	static bool defval(constDataPointer start) {
		return checkZeros(start, 2) && checkSpaces(start + 2, 13);
	}
};

struct placeRecord {
	Q_DECLARE_TR_FUNCTIONS(placeRecord)
public:
	Time entryTime;
	int entryTypeDailyWorkPeriod;
	int dailyWorkPeriodCountry;
	int dailyWorkPeriodRegion;
	int vehicleOdometerValue;

	placeRecord(constDataPointer start) :
		entryTime(BEInt32(start)), entryTypeDailyWorkPeriod(start[4]),
				dailyWorkPeriodCountry(start[5]), dailyWorkPeriodRegion(
						start[6]), vehicleOdometerValue(BEInt24(start + 7)) {
	}
	friend reporter& operator<<(reporter& report, placeRecord sub) {
		report(tr("entryTime"), sub.entryTime.str());
		report(tr("entryTypeDailyWorkPeriod"), formatStrings::dailyWorkPeriod(
				sub.entryTypeDailyWorkPeriod));
		report(tr("dailyWorkPeriodCountry"), formatStrings::nationNumeric(sub.dailyWorkPeriodCountry));
		if(sub.dailyWorkPeriodRegion) 
			report(tr("dailyWorkPeriodRegion"), sub.dailyWorkPeriodRegion);
		report(tr("vehicleOdometerValue"), sub.vehicleOdometerValue);
		return report;
	}
	static bool defval(constDataPointer start) {
		return checkZeros(start, 10);
	}
};

struct fullCardNumber {
	Q_DECLARE_TR_FUNCTIONS(fullCardNumber)
public:
	int Type;
	int Nation;
	QString Number;
	fullCardNumber(constDataPointer start) :
		Type(start[0]), Nation(start[1]), Number(fixedString(start + 2, 16)) {
	}
	friend reporter& operator<<(reporter& report, fullCardNumber sub) {
		report(tr("cardType"), formatStrings::equipmentType(sub.Type));
		report(tr("cardIssuingMemberState"), formatStrings::nationNumeric(sub.Nation));
		report(tr("cardNumber"), sub.Number);
		return report;
	}
	static bool defval(constDataPointer start) {
		return checkZeros(start, 3) && checkSpaces(start + 3, 16);
	}
};

class Control_Activity_Data : public tlvblock {
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
	Control_Activity_Data(constDataPointer filewalker) :
		tlvblock(filewalker), controlType(start[5]), controlTime(BEInt32(start
				+ 6)), controlCardNumber(start + 10),
				controlVehicleRegistration(start + 28),
				controlDownloadPeriodBegin(BEInt32(start + 43)),
				controlDownloadPeriodEnd(BEInt32(start + 47)) {
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

class Events_Data : public tlvblock {
	struct CardEventRecord {
		int Type;
		Time BeginTime, EndTime;
		vehicleRegistration vreg;
		CardEventRecord(constDataPointer start) :
			Type(start[0]), BeginTime(BEInt32(start + 1)), EndTime(BEInt32(
					start + 5)), vreg(start + 9) {
		}
		friend reporter& operator<<(reporter& report, CardEventRecord e) {
			report(tr("eventType"), formatStrings::eventType(e.Type));
			report(tr("beginTime"), e.BeginTime.str());
			report(tr("endTime"), e.EndTime.str());
			report << e.vreg;
			return report;
		}
		static bool defval(constDataPointer start) {
			return checkZeros(start, 9) && vehicleRegistration::defval(start
					+ 9);
		}

	};
	public:
	typedef std::vector<CardEventRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0502;
	virtual QString name() const {
		return tr("Events Data");
	}
	Events_Data(constDataPointer filewalker) :
		tlvblock(filewalker) {
		for(constDataPointer i = start + 5; i < start + 5 + datasize; i += 24) {
			if(!CardEventRecord::defval(i)) sub.push_back(CardEventRecord(i));
		}
	}
	virtual void printOn(reporter& o) const {
		for(subiter i = sub.begin(); i < sub.end(); ++i)
			o << *i;
	}
};

class Faults_Data : public Events_Data {
	public:
	Faults_Data(constDataPointer filewalker) :
		Events_Data(filewalker) {
	}
	virtual QString name() const {
		return tr("Faults Data");
	}
	static const int Type = 0x0503;
};

class Places : public tlvblock {
	public:
	typedef std::vector<placeRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0506;
	virtual QString name() const {
		return tr("Places");
	}
	Places(constDataPointer filewalker) :
		tlvblock(filewalker) {
		for(constDataPointer i = start + 5 + 1; i < start + 5 + datasize; i += 10) {
			if(!placeRecord::defval(i)) sub.push_back(placeRecord(i));
		}
	}
	virtual void printOn(reporter& o) const {
		o.reportraynosub(sub);
	}
};

class Vehicles_Used : public tlvblock {
	struct CardVehicleRecord {
		int OdometerBegin, OdometerEnd;
		Time FirstUse, LastUse;
		vehicleRegistration vreg;
		int vuDataBlockCounter;
		CardVehicleRecord(constDataPointer start) :
			OdometerBegin(BEInt24(start)), OdometerEnd(BEInt24(start + 3)),
					FirstUse(BEInt32(start + 6)), LastUse(BEInt32(start + 10)),
					vreg(start + 14), vuDataBlockCounter(BEInt16(start + 29)) {
		}
		friend reporter& operator<<(reporter& report,
				const CardVehicleRecord& e) {
			report << e.vreg;
			report(tr("vehicleFirstUse"), e.FirstUse.str());
			report(tr("vehicleLastUse"), e.LastUse.str());
			report(tr("vehicleOdometerBegin"), e.OdometerBegin);
			report(tr("vehicleOdometerEnd"), e.OdometerEnd);
			report(tr("vuDataBlockCounter"), e.vuDataBlockCounter);
			return report;
		}
		static bool defval(constDataPointer start) {
			return checkZeros(start, 14) && vehicleRegistration::defval(start
					+ 14) && checkZeros(start + 29, 2);
		}
	};
	public:
	typedef std::vector<CardVehicleRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0505;
	virtual QString name() const {
		return tr("Vehicles Used");
	}
	Vehicles_Used(constDataPointer filewalker) :
		tlvblock(filewalker) {
		for(constDataPointer i = start + 5 + 2; i < start + 5 + datasize; i += 31) {
			if(!CardVehicleRecord::defval(i)) sub.push_back(
					CardVehicleRecord(i));
		}
	}
	virtual void printOn(reporter& report) const {
		if(report.verbose) report.reportraynosub(sub);
		else {
			for(subiter i = sub.begin(); i < sub.end(); ++i) {
				subiter lastentry = i;
				while(i < (sub.end() - 1) && i->LastUse.timestamp % 86400 == 86399)
					++i;
				report << i->vreg;
				report.single(formatRange(lastentry->FirstUse, i->LastUse));
				report(tr("vehicleOdometerBegin"), lastentry->OdometerBegin);
				report(tr("vehicleOdometerEnd"), i->OdometerEnd);
				report(tr("Driven distance"), stringify(i->OdometerEnd
						- lastentry->OdometerBegin) + " km");
				if(i < sub.end()) report.blockbreak();
			}
		}
	}
};

class Identification : public tlvblock {
	public:
	int cardIssuingMemberState;
	QString cardNumber;
	QString cardIssuingAuthorityName;
	Time cardIssueDate;
	Time cardValidityBegin;
	Time cardExpiryDate;
	QString holderSurname;
	QString holderFirstname;
	QString cardHolderBirthDate;
	QString cardHolderPreferredLanguage;

	static const int Type = 0x0520;
	QString name() const {
		return tr("Card Identification");
	}
	Identification(constDataPointer filewalker) :
		tlvblock(filewalker), cardIssuingMemberState(start[5]), cardNumber(
				fixedString(start + 6, 16)), cardIssuingAuthorityName(
				fixedString(start + 22, 36)),
				cardIssueDate(BEInt32(start + 58)), cardValidityBegin(BEInt32(
						start + 62)), cardExpiryDate(BEInt32(start + 66)),
				holderSurname(fixedString(start + 70, 36)), holderFirstname(
						fixedString(start + 106, 36)), cardHolderBirthDate(
						BCDDate(start + 142)), cardHolderPreferredLanguage(
						fixedString(start + 146, 2)) {
	}
	virtual void printOn(reporter& o) const {
		o(tr("cardIssuingMemberState"), formatStrings::nationNumeric(cardIssuingMemberState));
		o(tr("cardNumber"), cardNumber);
		o(tr("cardIssuingAuthorityName"), cardIssuingAuthorityName);
		o(tr("cardIssueDate"), cardIssueDate.str());
		o(tr("cardValidityBegin"), cardValidityBegin.str());
		o(tr("cardExpiryDate"), cardExpiryDate.str());
		o(tr("holderSurname"), holderSurname);
		o(tr("holderFirstNames"), holderFirstname);
		o(tr("cardHolderBirthDate"), cardHolderBirthDate);
		o(tr("cardHolderPreferredLanguage"), cardHolderPreferredLanguage);
	}
	virtual void reportstuff(esmfilehead& esm) {
		esm.title = holderFirstname + " " + holderSurname;
	}
};

class Current_Usage : public tlvblock {
	public:
	static const int Type = 0x0507;
	QString name() const {
		return tr("Current Usage");
	}
	Time sessionOpenTime;
	int vehicleRegistrationNation;
	QString vehicleRegistrationNumber;

	Current_Usage(constDataPointer filewalker) :
		tlvblock(filewalker), sessionOpenTime(BEInt32(start + 5)),
				vehicleRegistrationNation(start[5 + 4]),
				vehicleRegistrationNumber(fixedString(start + 5 + 4 + 1, 14)) {
	}

	virtual void printOn(reporter& o) const {
		o(tr("sessionOpenTime"), sessionOpenTime.str());
		o(tr("vehicleRegistrationNation"), formatStrings::nationNumeric(vehicleRegistrationNation));
		o(tr("vehicleRegistrationNumber"), vehicleRegistrationNumber);
	}
};

///see p. 59
class Driver_Activity_Data : public tlvblock {
	public:
	static const int Type = 0x0504;
	QString name() const {
		return tr("Driver Activity Data");
	}
	Time LastCardDownload;
	int fine;
	int useddata;
	Driver_Activity_Data(constDataPointer filewalker) :
		tlvblock(filewalker), fine(0) {
		int oldestrec = BEInt16(start + 5); //start of oldest rec
		int newestrec = BEInt16(start + 5 + 2); //start of newest rec
		//newestrec += BEInt16(start + newestrec + 2); //end of newestrec
		useddata = newestrec - oldestrec;
		QByteArray actdata;
		qDebug() << "Driver_Activity_Data";
		if(newestrec < oldestrec) {
			useddata += datasize - 9;
			qDebug() << "older";
			//just copy the circular stuff into a new vector, avoids those boundary problems
			actdata.append(start.toPointer(9 + oldestrec), datasize - 4 - oldestrec);
			actdata.append(start.toPointer(9), newestrec - 4);
			//TODO: why 4? write at least a comment
		} else {
			qDebug() << "younger";
			actdata.append(start.toPointer(9 + oldestrec), newestrec - oldestrec);
		}
		constDataPointer walker(actdata,0);
		while(walker.bytesLeft() > 16) {
			qDebug() << walker.bytesLeft();
			int thissize = BEInt16(walker + 2);
			if(!thissize) {
				qDebug() << "Size 0 for increment. Aborting.";
				break;
			}
			DailyActivityCard d(walker, (thissize - 12) / 2);
			acts.push_back(d);
			walker += thissize;
		}
		fine = checkTimes(acts);
		qDebug() << "Driver_Activity_Data end";
	}
	typedef std::vector<DailyActivityCard> subray;
	typedef subray::const_iterator subiter;
	subray acts;

	virtual void printOn(reporter& o) const {
		o(tr("Accumulated fines"), stringify(fine) + " €");
		o(tr("Activity space usage"), stringify(useddata) + " " + tr("of") + " "
				+ stringify(datasize - 9) + " Bytes");
		o.reportraynosub(acts);

	}
	virtual void reportstuff(esmfilehead& esm) {
		for(subiter i = acts.begin(); i < acts.end(); ++i) {
			esm.reportDayStatistics(i->start, i->distance, i->driventime);
		}
	}

};

//Driver Card Codes and structure are on p. 119 in l207.pdf
tlvblock::ptr tlvblock::Factory(constDataPointer filewalker) {
	if(filewalker[2] == 1) qFatal("stray signature");
	typedef tlvblock::ptr p;
	switch(tlvblock::getType(filewalker)) {
		case Card_Download::Type:
			return p(new Card_Download(filewalker));
		case Driving_License_Info::Type:
			return p(new Driving_License_Info(filewalker));
		case Current_Usage::Type:
			return p(new Current_Usage(filewalker));
		case Driver_Activity_Data::Type:
			return p(new Driver_Activity_Data(filewalker));
		case Identification::Type:
			return p(new Identification(filewalker));
		case Specific_Conditions::Type:
			return p(new Specific_Conditions(filewalker));
		case Faults_Data::Type:
			return p(new Faults_Data(filewalker));
		case Events_Data::Type:
			return p(new Events_Data(filewalker));
		case Vehicles_Used::Type:
			return p(new Vehicles_Used(filewalker));
		case Card_Certificate::Type:
			return p(new Card_Certificate(filewalker));
		case CA_Certificate::Type:
			return p(new CA_Certificate(filewalker));
		case Application_Identification::Type:
			return p(new Application_Identification(filewalker));
		case Control_Activity_Data::Type:
			return p(new Control_Activity_Data(filewalker));
		case Places::Type:
			return p(new Places(filewalker));
		default:
			return p(new tlvblock(filewalker));
	}
}

#endif
