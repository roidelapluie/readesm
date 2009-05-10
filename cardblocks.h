/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef CARDBLOCKS_H
#define CARDBLOCKS_H
#include "Activity.h"
#include "esmfilehead.h"
#include "crypto.h"
#include "block.h"
#include "typedefs.h"
#include "overtime.h"

class tlvblock : public block{
	public:
	tlvblock(iter filewalker) : block(filewalker) {
		datasize =  (start[3] << 8) + start[4];
		filewalker = start + 5 + datasize;
		if(getType(filewalker) == type && filewalker[2] == 1){
			signature = filewalker + 5;
			hassignature = true;
		}
	}
	int size() const{
		return datasize + 5 + (hassignature ? 5 + 128 : 0);
	}
	static ptr Factory(iter& filewalker);
	virtual bool checksig(const rsa& key){
		if(hassignature) validsignature = CheckSignature(start + 5, datasize, signature, 128, key);
		return validsignature;
	}
	protected:
	int datasize;

};
class Driving_License_Info : public tlvblock{
	public:
	string drivingLicenseIssuingAuthorithy;
	static const int Type = 0x0521;
	int drivingLicenseIssuingNation;
	string drivingLicenseNumber;
	string name() const{ return "Driving License Info"; }
	Driving_License_Info(iter filewalker) : tlvblock(filewalker),
		drivingLicenseIssuingAuthorithy(fixedString(start + 5, 36)),
		drivingLicenseIssuingNation(start[5 + 36]),
		drivingLicenseNumber(fixedString(start + 5 + 36 + 1, 16)) {}

	virtual void printOn(reporter& o) const{
		o("drivingLicenceIssuingAuthorithy",drivingLicenseIssuingAuthorithy);
		o("drivingLicenceIssuingNation",nationNumeric(drivingLicenseIssuingNation));
		o("drivingLicenceNumber",drivingLicenseNumber);
	}
};

class Card_Download : public tlvblock{
	public:
	static const int Type = 0x050e;
	string name() const{ return "Card Download"; }
	Time LastCardDownload;
	Card_Download(iter filewalker) : tlvblock(filewalker),
		LastCardDownload(Time(BEInt32(filewalker + 5))){}
	virtual void printOn(reporter& o) const{
		o("LastCardDownload",LastCardDownload.str());
	}
};

class Application_Identification : public tlvblock{
	public:
	static const int Type = 0x0501;
	string name() const{ return "Application Identification"; }
	int typeOfTachographCardId, cardStructureVersion;
	Application_Identification(iter filewalker) : tlvblock(filewalker),
		typeOfTachographCardId(start[5]),
		cardStructureVersion(BEInt16(start + 6)){}
	virtual void printOn(reporter& o) const{
		o("typeOfTachographCardId",formatEquipmentType(typeOfTachographCardId));
		o("cardStructureVersion",hex(cardStructureVersion, 4));
		//TODO: read noOfCompanyActivityRecords
	}
};

class Card_Certificate : public tlvblock{
	public:
	static const int Type = 0xc100;
	virtual string name() const{ return "Card Certificate"; }
	Card_Certificate(iter filewalker) : tlvblock(filewalker){}
	virtual void reportstuff(esmfilehead& esm){
		if(esm.devicecert) std::cerr << "Reassigning cert\n";
		esm.devicecert = verifiedcert::ptr(new verifiedcert(start + 5));
	}
};

class CA_Certificate : public Card_Certificate{
	public:
	static const int Type = 0xc108;
	virtual string name() const{ return "CA Certificate"; }
	CA_Certificate(iter filewalker) : Card_Certificate(filewalker){}
	virtual void reportstuff(esmfilehead& esm){
		if(esm.CAcert) std::cerr << "Reassigning cert\n";
		esm.CAcert = verifiedcert::ptr(new verifiedcert(start + 5));
	}
};

class Specific_Conditions : public tlvblock{
	struct Specific_Condition{
		Time time;
		int condition;
		Specific_Condition(Time time_, int condition_) : time(time_), condition(condition_) {} 
		friend reporter& operator<<(reporter& report, Specific_Condition scond){
			report(scond.time.str().c_str() ,formatSpecificCondition(scond.condition));
			return report;
		}
	};
	public:
	typedef std::vector<Specific_Condition> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0522;
	string name() const{ return "Specific Conditions"; }
	Specific_Conditions(iter filewalker) : tlvblock(filewalker){
		for(iter i = start + 5; i < start + 5 + datasize; i += 5){
			int time = BEInt32(i), cond = i[4];
			if(time != 0 || cond != 0) sub.push_back(Specific_Condition(time, cond));
		}
	}
	virtual void printOn(reporter& o) const{
		for(subiter i = sub.begin(); i < sub.end(); ++i) o << *i;
	}
};




struct vehicleRegistration{
	int Nation;
	string Number;
	vehicleRegistration(iter start) : Nation(start[0]), Number(fixedString(start + 1, 14)) {} 
	friend reporter& operator<<(reporter& report, vehicleRegistration sub){
		if(report.verbose) report("vehicleRegistrationNation" ,nationNumeric(sub.Nation));
		report("vehicleRegistrationNumber" ,sub.Number);
		return report;
	}
	static bool defval(iter start){
		return checkZeros(start, 2)  && checkSpaces(start + 2, 13);
	}
};

struct placeRecord{
	Time entryTime;
	int entryTypeDailyWorkPeriod;
	int dailyWorkPeriodCountry;
	int dailyWorkPeriodRegion;
	int vehicleOdometerValue;

	placeRecord(iter start) : 
		entryTime(BEInt32(start)),
		entryTypeDailyWorkPeriod(start[4]),
		dailyWorkPeriodCountry(start[5]),
		dailyWorkPeriodRegion(start[6]),
		vehicleOdometerValue(BEInt24(start + 7))
		{}
	friend reporter& operator<<(reporter& report, placeRecord sub){
		report("entryTime",sub.entryTime.str());
		report("entryTypeDailyWorkPeriod" ,formatDailyWorkPeriod(sub.entryTypeDailyWorkPeriod));
		report("dailyWorkPeriodCountry" ,nationNumeric(sub.dailyWorkPeriodCountry));
		report("dailyWorkPeriodRegion",sub.dailyWorkPeriodRegion);
		report("vehicleOdometerValue",sub.vehicleOdometerValue);
		return report;
	}
	static bool defval(iter start){
		return checkZeros(start, 10);
	}
};



struct fullCardNumber{
	int Type;
	int Nation;
	string Number;
	fullCardNumber(iter start) : 
		Type(start[0]), 
		Nation(start[1]), 
		Number(fixedString(start + 2, 16)) {} 
	friend reporter& operator<<(reporter& report, fullCardNumber sub){
		report("cardType", formatEquipmentType(sub.Type));
		report("cardIssuingMemberState", nationNumeric(sub.Nation));
		report("cardNumber", sub.Number);
		return report;
	}
	static bool defval(iter start){
		return checkZeros(start, 3)  && checkSpaces(start + 3, 16);
	}
};

class Control_Activity_Data : public tlvblock{
	public:
	static const int Type = 0x0508;
	int controlType;
	Time controlTime;
	fullCardNumber controlCardNumber;
	vehicleRegistration controlVehicleRegistration;
	Time controlDownloadPeriodBegin, controlDownloadPeriodEnd;
	virtual string name() const{ return "Control Activity Data"; }
	Control_Activity_Data(iter filewalker) : tlvblock(filewalker),
		controlType(start[5]),
		controlTime(BEInt32(start + 6)),
		controlCardNumber(start + 10),
		controlVehicleRegistration(start + 28),
		controlDownloadPeriodBegin(BEInt32(start + 43)),
		controlDownloadPeriodEnd(BEInt32(start + 47)) {}
	virtual void printOn(reporter& o) const{
		o("controlType", formatControlType(controlType));
		o("controlTime", controlTime.str());
		o << controlCardNumber;
		o << controlVehicleRegistration;
		o("controlDownloadPeriodBegin", controlDownloadPeriodBegin.str());
		o("controlDownloadPeriodEnd", controlDownloadPeriodEnd.str());
	}
};


class Events_Data : public tlvblock{
	struct CardEventRecord{
		int Type;
		Time BeginTime, EndTime;
		vehicleRegistration vreg;
		CardEventRecord(iter start) : Type(start[0]), BeginTime(BEInt32(start + 1)), EndTime(BEInt32(start + 5)), vreg(start+9) {} 
		friend reporter& operator<<(reporter& report, CardEventRecord e){
			report("eventType" ,formatEventType(e.Type));
			report("beginTime" ,e.BeginTime.str());
			report("endTime" ,e.EndTime.str());
			report << e.vreg;
			return report;
		}
		static bool defval(iter start){
//			hexout(std::cout, start,24); std::cout << "\n";
			return checkZeros(start, 9)  && vehicleRegistration::defval(start + 9);
		}

	};
	public:
	typedef std::vector<CardEventRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0502;
	virtual string name() const{ return "Events Data"; }
	Events_Data(iter filewalker) : tlvblock(filewalker){
		for(iter i = start + 5; i < start + 5 + datasize; i += 24){
			if(!CardEventRecord::defval(i)) sub.push_back(CardEventRecord(i));
		}
	}
	virtual void printOn(reporter& o) const{
		for(subiter i = sub.begin(); i < sub.end(); ++i) o << *i;
	}
};

class Faults_Data : public Events_Data{
	public:
	Faults_Data(iter filewalker) : Events_Data(filewalker){}
	virtual string name() const{ return "Faults Data"; }
	static const int Type = 0x0503;
};

class Places : public tlvblock{
	public:
	typedef std::vector<placeRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0506;
	virtual string name() const{ return "Places"; }
	Places(iter filewalker) : tlvblock(filewalker){
		for(iter i = start + 5 + 1; i < start + 5 + datasize; i += 10){
			if(!placeRecord::defval(i)) sub.push_back(placeRecord(i));
		}
	}
	virtual void printOn(reporter& o) const{
		o.reportraynosub(sub);
		//for(subiter i = sub.begin(); i < sub.end(); ++i) o << *i;
	}
};


class Vehicles_Used : public tlvblock{
	struct CardVehicleRecord{
		int OdometerBegin, OdometerEnd;
		Time FirstUse, LastUse;
		vehicleRegistration vreg;
		int vuDataBlockCounter;
		CardVehicleRecord(iter start) : 
			OdometerBegin(BEInt24(start)),
			OdometerEnd(BEInt24(start + 3)),
			FirstUse(BEInt32(start + 6)),
			LastUse(BEInt32(start + 10)),
			vreg(start+14),
			vuDataBlockCounter(BEInt16(start + 29)) {} 
		friend reporter& operator<<(reporter& report, const CardVehicleRecord& e){
			report("vehicleOdometerBegin" ,e.OdometerBegin);
			report("vehicleOdometerEnd" ,e.OdometerEnd);
			report("vehicleFirstUse" ,e.FirstUse.str());
			report("vehicleLastUse" ,e.LastUse.str());
			report << e.vreg;
			report("vuDataBlockCounter" ,e.vuDataBlockCounter);
			return report;
		}
		static bool defval(iter start){
			return checkZeros(start, 14)  && vehicleRegistration::defval(start + 14) && checkZeros(start + 29, 2);
		}
	};
	public:
	typedef std::vector<CardVehicleRecord> subray;
	typedef subray::const_iterator subiter;
	subray sub;
	static const int Type = 0x0505;
	virtual string name() const{ return "Vehicles Used"; }
	Vehicles_Used(iter filewalker) : tlvblock(filewalker){
		for(iter i = start + 5 + 2; i < start + 5 + datasize; i += 31){
			if(!CardVehicleRecord::defval(i)) sub.push_back(CardVehicleRecord(i));
		}
	}
	virtual void printOn(reporter& o) const{
		o.reportraynosub(sub);
		//for(subiter i = sub.begin(); i < sub.end(); ++i) o << *i;
	}
};

class Identification : public tlvblock{
	public:
	int cardIssuingMemberState;
	string cardNumber;
	string cardIssuingAuthorityName;
	Time cardIssueDate;
	Time cardValidityBegin;
	Time cardExpiryDate;
	string holderSurname;
	string holderFirstname;
	string cardHolderBirthDate;
	string cardHolderPreferredLanguage;

	static const int Type = 0x0520;
	string name() const{ return "Card Identification"; }
	Identification(iter filewalker) : tlvblock(filewalker),
		cardIssuingMemberState(start[5]),
		cardNumber(fixedString(start + 6, 16)),
		cardIssuingAuthorityName(fixedString(start + 22, 36)),
		cardIssueDate(BEInt32(start + 58)),
		cardValidityBegin(BEInt32(start + 62)),
		cardExpiryDate(BEInt32(start + 66)),
		holderSurname(fixedString(start + 70, 36)),
		holderFirstname(fixedString(start + 106, 36)),
		cardHolderBirthDate(BCDDate(start + 142)),
		cardHolderPreferredLanguage(fixedString(start + 146, 2))
		{}
	virtual void printOn(reporter& o) const{
		o("cardIssuingMemberState",nationNumeric(cardIssuingMemberState));
		o("cardNumber",cardNumber);
		o("cardIssuingAuthorityName",cardIssuingAuthorityName);
		o("cardIssueDate",cardIssueDate.str());
		o("cardValidityBegin",cardValidityBegin.str());
		o("cardExpiryDate",cardExpiryDate.str());
		o("holderSurname",holderSurname);
		o("holderFirstNames",holderFirstname);
		o("cardHolderBirthDate",cardHolderBirthDate);
		o("cardHolderPreferredLanguage",cardHolderPreferredLanguage);
	}
	virtual void reportstuff(esmfilehead& esm){
		esm.title = holderFirstname + " " + holderSurname;
	}
};


class Current_Usage : public tlvblock{
	public:
	static const int Type = 0x0507;
	string name() const{ return "Current Usage"; }
	Time sessionOpenTime;
	int vehicleRegistrationNation;
	string vehicleRegistrationNumber;

	Current_Usage(iter filewalker) : tlvblock(filewalker),
		sessionOpenTime(BEInt32(start + 5)),
		vehicleRegistrationNation(start[5+4]),
		vehicleRegistrationNumber(fixedString(start + 5+4+1, 14)) {}
		
	virtual void printOn(reporter& o) const{
		o("sessionOpenTime",sessionOpenTime.str());
		o("vehicleRegistrationNation",nationNumeric(vehicleRegistrationNation));
		o("vehicleRegistrationNumber",vehicleRegistrationNumber);
	}
};

/*
class circular_accessor{
	iter datap;
	int period_;
	int index;
	inline int mathmod(int nominator, int denom){
		int rv = nominator % denom;
		return rv > 0? rv:rv += period;
	}
	public:
	circular_accessor(iter  start, int length, int index_ = 0): datap(start), period_(length), index(index_) {};
	int period() const { return period_; }
	unsigned char operator[](int index) const{ return datap[mathmod(index, period_)]; }
};*/
///p. 57 and corrigendum(!)
class DailyActivityCard : public DailyActivity {
	public:
	int presence, distance;
	DailyActivityCard(iter start, int count) : DailyActivity(start + 4, start + 12, count), presence(BEInt16(start + 8)), distance(BEInt16(start + 10)) {}
	friend reporter& operator<<(reporter& o, const DailyActivityCard& d){
		o << (DailyActivity)d;
		if(o.verbose) o("activityDailyPresenceCounter",d.presence);
		if(d.distance) o("activityDayDistance",d.distance);
		return o;
	}
};

///see p. 59
class Driver_Activity_Data : public tlvblock{
	public:
	static const int Type = 0x0504;
	string name() const{ return "Driver Activity Data"; }
	Time LastCardDownload;
	int fine;
	int useddata;
	Driver_Activity_Data(iter filewalker) : tlvblock(filewalker), fine(0) {
		int oldestrec = BEInt16(start + 5); //start of oldest rec
		int newestrec = BEInt16(start + 5 + 2); //start of newest rec
		//newestrec += BEInt16(start + newestrec + 2); //end of newestrec
		useddata = newestrec - oldestrec;
		slurpedfile actdata;
		iter walker, end;
		if(newestrec < oldestrec) {
			useddata += datasize - 9;
			//just copy the circular stuff into a new vector, avoids those boundary problems
			actdata.reserve(datasize - 9);
			for(iter i = start + 9 + oldestrec; i < start + 5 + datasize; ++i) actdata.push_back(*i);
			for(iter i = start + 9; i < start + 5 + newestrec; ++i) actdata.push_back(*i);
			walker = actdata.begin();
			end = actdata.end();
		} else {
			walker = start + 9 + oldestrec;
			end = start + 9 + newestrec;
		}
		while(walker < end){
			int thissize = BEInt16(walker + 2);
			if(!thissize){
				std::cerr << "Size 0 for increment. Aborting.\n";
				break;
			}
			DailyActivityCard d(walker, (thissize - 12)/2);
			checkDayDrivingTime(d);
			fine += d.fine;
			acts.push_back(d);
			walker += thissize;
		}

	}
	typedef std::vector<DailyActivityCard> subray;
	typedef subray::const_iterator subiter;
	subray acts;

	virtual void printOn(reporter& o) const{
		o("Accumulated fines", fine);
		o("Activity space usage", stringify(useddata) + " of " + stringify(datasize - 9) + " Bytes");
		o.reportraynosub(acts);
		
	}
	virtual void reportstuff(esmfilehead& esm){
		for(subiter i = acts.begin(); i < acts.end(); ++i){
			esm.reportDayStatistics(i->start, i->distance, i->driventime);
		}
	}

};

//Driver Card Codes and structure are on p. 119 in l207.pdf
tlvblock::ptr tlvblock::Factory(iter& filewalker){
	if(filewalker[2] == 1) throw std::runtime_error("stray signature");
	typedef tlvblock::ptr p;
	switch(tlvblock::getType(filewalker)){
		case Card_Download::Type	: return p(new Card_Download(filewalker));
		case Driving_License_Info::Type	: return p(new Driving_License_Info(filewalker));
		case Current_Usage::Type	: return p(new Current_Usage(filewalker));
		case Driver_Activity_Data::Type	: return p(new Driver_Activity_Data(filewalker));
		case Identification::Type	: return p(new Identification(filewalker));
		case Specific_Conditions::Type	: return p(new Specific_Conditions(filewalker));
		case Faults_Data::Type		: return p(new Faults_Data(filewalker));
		case Events_Data::Type		: return p(new Events_Data(filewalker));
		case Vehicles_Used::Type	: return p(new Vehicles_Used(filewalker));
		case Card_Certificate::Type	: return p(new Card_Certificate(filewalker));
		case CA_Certificate::Type	: return p(new CA_Certificate(filewalker));
		case Application_Identification::Type:return p(new Application_Identification(filewalker));
		case Control_Activity_Data::Type: return p(new Control_Activity_Data(filewalker));
		case Places::Type               : return p(new Places(filewalker));
		default: return p(new tlvblock(filewalker));
	}
}

#endif
