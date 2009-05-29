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

#ifndef FORMATSTRINGS_H
#define FORMATSTRINGS_H
#include "typedefs.h"
#include "i18n.h"

string formatEventType(unsigned char etype) {
	ostringstream o;
	int major = etype >> 4;
	int minor = etype & 0xF;
	o << int(etype) << " - ";
	if(major == 0) {
		o << tr("General events: ");
		const char* gevents[] = { "No further details",
				"Insertion of a non-valid card", "Card conflict",
				"Time overlap", "Driving without an appropriate card",
				"Card insertion while driving",
				"Last card session not correctly closed", "Over speeding",
				"Power supply interruption", "Motion data error" };
		if(minor <= 0x9) o << tr(gevents[minor]);
		else o << tr("RFU");
	} else if(major == 1) {
		o << tr("Vehicle unit related security breach attempt events: ");
		const char* sevents[] = { "No further details",
				"Motion sensor authentication failure",
				"Tachograph card authentication failure",
				"Unauthorised change of motion sensor",
				"Card data input integrity error",
				"Stored user data integrity error",
				"Internal data transfer error", "Unauthorised case opening",
				"Hardware sabotage" };
		if(minor <= 0x8) o << tr(sevents[minor]);
		else o << tr("RFU");
	} else if(major == 2) {
		o << tr("Sensor related security breach attempt events: ");
		const char* sevents[] = { "No further details",
				"Authentication failure", "Stored data integrity error",
				"Internal data transfer error", "Unauthorised case opening",
				"Hardware sabotage" };
		if(minor <= 0x5) o << tr(sevents[minor]);
		else o << tr("RFU");
	} else if(major == 3) {
		o << tr("Recording equipment faults: ");
		const char* sevents[] = { "No further details", "VU internal fault",
				"Printer fault", "Display fault", "Downloading fault",
				"Sensor fault" };
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
	return o.str();
}

string formatEventRecordPurpose(unsigned char etype) {
	const char
			* rpurp[] = {
					"one of the 10 most recent (or last) events or faults",
					"the longest event for one of the last 10 days of occurrence",
					"one of the 5 longest events over the last 365 days",
					"the last event for one of the last 10 days of occurrence",
					"the most serious event for one of the last 10 days of occurrence",
					"one of the 5 most serious events over the last 365 days",
					"the first event or fault having occurred after the last calibration",
					"an active/on-going event or fault" };
	if(etype <= 7) return tr(rpurp[etype]);
	else if(8 <= etype && etype <= 0x7F) {
		return tr("RFU");
	} else if(0x80 <= etype && etype <= 0x7F) {
		return tr("Manufacturer specific");
	} else return tr("(err:blame programmer)");
}

string nationNumeric(unsigned char country) {
	static const char* countries[] = { "No information available", "Austria",
			"Albania", "Andorra", "Armenia", "Azerbaijan", "Belgium",
			"Bulgaria", "Bosnia and Herzegovina", "Belarus", "Switzerland",
			"Cyprus", "Czech Republic", "Germany", "Denmark", "Spain",
			"Estonia", "France", "Finland", "Liechtenstein", "Faeroe Islands",
			"United Kingdom", "Georgia", "Greece", "Hungary", "Croatia",
			"Italy", "Ireland", "Iceland", "Kazakhstan", "Luxembourg",
			"Lithuania", "Latvia", "Malta", "Monaco", "Republic of Moldova",
			"Macedonia", "Norway", "Netherlands", "Portugal", "Poland",
			"Romania", "San Marino", "Russian Federation", "Sweden",
			"Slovakia", "Slovenia", "Turkmenistan", "Turkey", "Ukraine",
			"Vatican City", "Yugoslavia" };
	if(country <= 0x33) return tr(countries[country]);
	if(0x34 <= country && country <= 0xFC) return tr("Reserved for future use");
	if(country == 0xFD) return tr("European Community");
	if(country == 0xFE) return tr("Europe, but not EC and not registered");
	if(country == 0xFF) return tr("outside of Europe, not registered");
	return tr("error in nationNumeric");
}

string formatSpecificCondition(unsigned char country) {
	static const char* countries[] = { "RFU", "Out of scope – Begin",
			"Out of scope – End", "Ferry/Train crossing" };
	if(country <= 0x3) return tr(countries[country]);
	else return tr("RFU");
}
string formatCalibrationPurpose(unsigned char cpurp) {
	const char
			* cpurps[] = {
					"reserved value (should not appear)",
					"activation: recording of calibration parameters known, at the moment of the VU activation",
					"first installation: first calibration of the VU after its activation",
					"installation: first calibration of the VU in the current vehicle",
					"periodic inspection" };
	if(cpurp <= 4) return tr(cpurps[cpurp]);
	else return tr("(not specified)");
}

string formatControlType(unsigned char ctype) {
	ostringstream o;
	if(ctype & (1 << 7)) o << tr("card downloaded, ");
	if(ctype & (1 << 6)) o << tr("VU downloaded, ");
	if(ctype & (1 << 5)) o << tr("printing done, ");
	if(ctype & (1 << 4)) o << tr("display used, ");
	return o.str();
}

string formatEquipmentType(unsigned char value) {
	const char* vals[] = { "Reserved", "Driver Card", "Workshop Card",
			"Control Card", "Company Card", "Manufacturing Card",
			"Vehicle Unit", "Motion Sensor" };
	if(value <= 7) return tr(vals[value]);
	else return tr("RFU");
}

string formatDailyWorkPeriod(unsigned char value) {
	const char* vals[] = {
			"Begin, related time = card insertion time or time of entry",
			"End,   related time = card withdrawal time or time of entry",
			"Begin, related time manually entered (start time)",
			"End,   related time manually entered (end of work period)",
			"Begin, related time assumed by VU",
			"End,   related time assumed by VU ", };
	if(value <= 5) return tr(vals[value]);
	else return tr("(not specified)");
}

#endif
