/* Copyright 2011 Andreas Gölzer

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

#include "formatStrings.h"

#include <QtCore/QTextStream>

QString formatStrings::eventType(unsigned char etype) {
	QString rv;
	QTextStream o(&rv);
	int major = etype >> 4;
	int minor = etype & 0xF;
	o << int(etype) << " - ";
	if(major == 0) {
		o << tr("General events: ");
		const char* generalEvents[] = { 
			QT_TR_NOOP("No further details"),
			QT_TR_NOOP("Insertion of a non-valid card"), 
			QT_TR_NOOP("Card conflict"),
			QT_TR_NOOP("Time overlap"), 
			QT_TR_NOOP("Driving without an appropriate card"),
			QT_TR_NOOP("Card insertion while driving"),
			QT_TR_NOOP("Last card session not correctly closed"), 
			QT_TR_NOOP("Over speeding"),
			QT_TR_NOOP("Power supply interruption"), 
			QT_TR_NOOP("Motion data error")
		};
		if(minor <= 0x9) o << tr(generalEvents[minor]);
		else o << tr("RFU");
	} else if(major == 1) {
		o << tr("Vehicle unit related security breach attempt events: ");
		const char* sevents[] = { 
			QT_TR_NOOP("No further details"),
			QT_TR_NOOP("Motion sensor authentication failure"),
			QT_TR_NOOP("Tachograph card authentication failure"),
			QT_TR_NOOP("Unauthorised change of motion sensor"),
			QT_TR_NOOP("Card data input integrity error"),
			QT_TR_NOOP("Stored user data integrity error"),
			QT_TR_NOOP("Internal data transfer error"), 
			QT_TR_NOOP("Unauthorised case opening"),
			QT_TR_NOOP("Hardware sabotage") 
		};
		if(minor <= 0x8) o << tr(sevents[minor]);
		else o << tr("RFU");
	} else if(major == 2) {
		o << tr("Sensor related security breach attempt events: ");
		const char* sevents[] = { 
			QT_TR_NOOP("No further details"),
			QT_TR_NOOP("Authentication failure"), 
			QT_TR_NOOP("Stored data integrity error"),
			QT_TR_NOOP("Internal data transfer error"), 
			QT_TR_NOOP("Unauthorised case opening"),
			QT_TR_NOOP("Hardware sabotage") };
		if(minor <= 0x5) o << tr(sevents[minor]);
		else o << tr("RFU");
	} else if(major == 3) {
		o << tr("Recording equipment faults: ");
		const char* sevents[] = { 
			QT_TR_NOOP("No further details"), 
			QT_TR_NOOP("VU internal fault"),
			QT_TR_NOOP("Printer fault"), 
			QT_TR_NOOP("Display fault"), 
			QT_TR_NOOP("Downloading fault"),
			QT_TR_NOOP("Sensor fault") 
		};
		if(minor <= 0x5) o << tr(sevents[minor]);
		else o << tr("RFU");
	} else if(major == 4) {
		o << tr("Card faults: ");
		const char* sevents[] = { "No further details" };
		if(minor <= 0x1) o << tr(sevents[minor]);
		else o << tr("RFU");
	} else if(major >= 5 && major <= 7) {
		o << tr("RFU groups");
	} else if(major >= 8 && major <= 0xF) {
		o << tr("Manufacturer specific");
	} else o << tr("(error: blame programmer)");
	return rv;
}

QString formatStrings::eventRecordPurpose(unsigned char etype) {
	const char* recordingPurposes[] = {
		QT_TR_NOOP("one of the 10 most recent (or last) events or faults"),
		QT_TR_NOOP("the longest event for one of the last 10 days of occurrence"),
		QT_TR_NOOP("one of the 5 longest events over the last 365 days"),
		QT_TR_NOOP("the last event for one of the last 10 days of occurrence"),
		QT_TR_NOOP("the most serious event for one of the last 10 days of occurrence"),
		QT_TR_NOOP("one of the 5 most serious events over the last 365 days"),
		QT_TR_NOOP("the first event or fault having occurred after the last calibration"),
		QT_TR_NOOP("an active/on-going event or fault")
	};
	if(etype <= 7) return tr(recordingPurposes[etype]);
	else if(8 <= etype && etype <= 0x7F) {
		return tr("RFU");
	} else if(0x80 <= etype && etype <= 0x7F) {
		return tr("Manufacturer specific");
	} else return tr("(err:blame programmer)");
}

QString formatStrings::nationNumeric(unsigned char country) {
	 const char* countries[] = { 
		QT_TR_NOOP("No information available"), 
		QT_TR_NOOP("Austria"),
		QT_TR_NOOP("Albania"), 
		QT_TR_NOOP("Andorra"), 
		QT_TR_NOOP("Armenia"), 
		QT_TR_NOOP("Azerbaijan"), 
		QT_TR_NOOP("Belgium"),
		QT_TR_NOOP("Bulgaria"), 
		QT_TR_NOOP("Bosnia and Herzegovina"), 
		QT_TR_NOOP("Belarus"), 
		QT_TR_NOOP("Switzerland"),
		QT_TR_NOOP("Cyprus"), 
		QT_TR_NOOP("Czech Republic"), 
		QT_TR_NOOP("Germany"), 
		QT_TR_NOOP("Denmark"), 
		QT_TR_NOOP("Spain"),
		QT_TR_NOOP("Estonia"), 
		QT_TR_NOOP("France"), 
		QT_TR_NOOP("Finland"), 
		QT_TR_NOOP("Liechtenstein"), 
		QT_TR_NOOP("Faeroe Islands"),
		QT_TR_NOOP("United Kingdom"), 
		QT_TR_NOOP("Georgia"), 
		QT_TR_NOOP("Greece"), 
		QT_TR_NOOP("Hungary"), 
		QT_TR_NOOP("Croatia"),
		QT_TR_NOOP("Italy"), 
		QT_TR_NOOP("Ireland"), 
		QT_TR_NOOP("Iceland"), 
		QT_TR_NOOP("Kazakhstan"), 
		QT_TR_NOOP("Luxembourg"),
		QT_TR_NOOP("Lithuania"), 
		QT_TR_NOOP("Latvia"), 
		QT_TR_NOOP("Malta"), 
		QT_TR_NOOP("Monaco"), 
		QT_TR_NOOP("Republic of Moldova"),
		QT_TR_NOOP("Macedonia"), 
		QT_TR_NOOP("Norway"), 
		QT_TR_NOOP("Netherlands"), 
		QT_TR_NOOP("Portugal"), 
		QT_TR_NOOP("Poland"),
		QT_TR_NOOP("Romania"), 
		QT_TR_NOOP("San Marino"), 
		QT_TR_NOOP("Russian Federation"), 
		QT_TR_NOOP("Sweden"),
		QT_TR_NOOP("Slovakia"), 
		QT_TR_NOOP("Slovenia"), 
		QT_TR_NOOP("Turkmenistan"), 
		QT_TR_NOOP("Turkey"), 
		QT_TR_NOOP("Ukraine"),
		QT_TR_NOOP("Vatican City"), 
		QT_TR_NOOP("Yugoslavia"),
		QT_TR_NOOP("Montenegro"), //new ones only found on dtc.jrc.it
		QT_TR_NOOP("Serbia"),
		QT_TR_NOOP("Uzbekistan")
	};
	if(country <= 0x36) return tr(countries[country]);
	if(country <= 0xFC) return tr("Reserved for future use");
	if(country == 0xFD) return tr("European Community");
	if(country == 0xFE) return tr("Europe, but not EC and not registered");
	if(country == 0xFF) return tr("outside of Europe, not registered");
	return tr("error in nationNumeric");
}

QString formatStrings::specificCondition(unsigned char scond) {
	const char* specificConditions[] = {
		QT_TR_NOOP("RFU"), 
		QT_TR_NOOP("Out of scope - Begin"),
		QT_TR_NOOP("Out of scope - End"),
		QT_TR_NOOP("Ferry/Train crossing")
	};
	if(scond <= 0x3) return tr(specificConditions[scond]);
	else return tr("RFU");
}

QString formatStrings::calibrationPurpose(unsigned char cpurp) {
	const char * cpurps[] = {
		QT_TR_NOOP("reserved value (should not appear)"),
		QT_TR_NOOP("activation: recording of calibration parameters known, at the moment of the VU activation"),
		QT_TR_NOOP("first installation: first calibration of the VU after its activation"),
		QT_TR_NOOP("installation: first calibration of the VU in the current vehicle"),
		QT_TR_NOOP("periodic inspection") 
	};
	if(cpurp <= 4) return tr(cpurps[cpurp]);
	else return tr("(not specified)");
}

QString formatStrings::controlType(unsigned char ctype) {
	QString rv;
	QTextStream o(&rv);
	if(ctype & (1 << 7)) o << tr("card downloaded") << ", ";
	if(ctype & (1 << 6)) o << tr("VU downloaded") << ", ";
	if(ctype & (1 << 5)) o << tr("printing done") << ", ";
	if(ctype & (1 << 4)) o << tr("display used") << ", ";
	return rv;
}

QString formatStrings::equipmentType(unsigned char value) {
	const char* vals[] = {
		QT_TR_NOOP("Reserved"), 
		QT_TR_NOOP("Driver Card"), 
		QT_TR_NOOP("Workshop Card"),
		QT_TR_NOOP("Control Card"), 
		QT_TR_NOOP("Company Card"), 
		QT_TR_NOOP("Manufacturing Card"),
		QT_TR_NOOP("Vehicle Unit"), 
		QT_TR_NOOP("Motion Sensor")
	};
	if(value <= 7) return tr(vals[value]);
	else return tr("RFU");
}

QString formatStrings::dailyWorkPeriod(unsigned char value) {
	const char* vals[] = {
		QT_TR_NOOP("Begin, related time = card insertion time or time of entry"),
		QT_TR_NOOP("End, related time = card withdrawal time or time of entry"),
		QT_TR_NOOP("Begin, related time manually entered (start time)"),
		QT_TR_NOOP("End, related time manually entered (end of work period)"),
		QT_TR_NOOP("Begin, related time assumed by VU"),
		QT_TR_NOOP("End, related time assumed by VU ")
	};
	if(value <= 5) return tr(vals[value]);
	else return tr("(not specified)");
}

QString formatStrings::regionNumeric(unsigned char value) {
	const char* vals[] = {
		QT_TR_NOOP("No information available"),
		QT_TR_NOOP("Andalucía"),
		QT_TR_NOOP("Aragón"),
		QT_TR_NOOP("Asturias"),
		QT_TR_NOOP("Cantabria"),
		QT_TR_NOOP("Cataluña"),
		QT_TR_NOOP("Castilla-León"),
		QT_TR_NOOP("Castilla-La-Mancha"),
		QT_TR_NOOP("Valencia"),
		QT_TR_NOOP("Extremadura"),
		QT_TR_NOOP("Galicia"),
		QT_TR_NOOP("Baleares"),
		QT_TR_NOOP("Canarias"),
		QT_TR_NOOP("La Rioja"),
		QT_TR_NOOP("Madrid"),
		QT_TR_NOOP("Murcia"),
		QT_TR_NOOP("Navarra"),
		QT_TR_NOOP("País Vasco")
	};
	if(value <= 0x11) return tr(vals[value]);
	else return tr("(unknown region)");
}

///Manufacturer codes as found on dtc.jrc.it
QString formatStrings::manufacturerCode(unsigned char code) {
	switch(code){
	case 0x00: return tr("No information available");
	case 0x01: return tr("Reserved value");
	case 0x10: return tr("Actia S.A.");
	case 0x12: return tr("Austria Card Plastikkarten und Ausweissysteme GmbH");
	case 0x13: return tr("Agencija za komercijalnu djelatnost d.o.o (AKD)");
	case 0x20: return tr("CETIS d.d.");
	case 0x21: return tr("certSIGN");
	case 0x22: return tr("RUE Cryptotech");
	case 0x30: return tr("Sdu Identification B.V. (formerly Enschedé/Sdu B.V.)");
	case 0x32: return tr("EFKON AG.");
	case 0x38: return tr("Fábrica Nacional de Moneda y Timbre");
	case 0x40: return tr("Giesecke & Devrient GmbH");
	case 0x43: return tr("Giesecke & Devrient GB Ltd.");
	case 0x44: return tr("Giesecke & Devrient sa/nv");
	case 0x48: return tr("Hungarian Banknote Printing Co. Ltd.");
	case 0x50: return tr("Imprimerie Nationale");
	case 0x51: return tr("Imprensa Nacional-Casa da Moeda, SA");
	case 0x52: return tr("InfoCamere S.C.p.A");
	case 0x81: return tr("Morpho e-documents (formerly Sagem Orga formerly ORGA Kartensysteme GmbH)");
	case 0x82: return tr("ORGA Zelenograd ZAO");
	case 0x88: return tr("Asseco Czech Republic a.s. (formerly PVT a.s.)");
	case 0x89: return tr("Polska Wytwórnia Papierów Wartosciowych S.A. - PWPW S.A.");
	case 0xA1: return tr("Continental Automotive GmbH (formerly Siemens AG - Siemens VDO Automotive Siemens Automotive)"); 
	case 0xA2: return tr("Stoneridge Electronics AB");
	case 0xA3: return tr("Gemalto (formerly Schlumberger SEMA, Axalto)");
	case 0xA4: return tr("3M Security Printing and Systems Ltd.");
	case 0xD8: return tr("Union of Chambers and Commodity Exchanges of Turkey - TOBB");
	case 0xAB: return tr("T-Systems International GmbH");
	case 0xAC: return tr("Trüb AG");
	case 0xAD: return tr("Trüb Baltic AS");
	case 0xAE: return tr("TEMPEST a.s.");
	case 0xAF: return tr("Trueb - DEMAX PLC");
	}
	return tr("Unknown Manufacturer or equipment not type approved");
}
