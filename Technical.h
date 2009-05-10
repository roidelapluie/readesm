/* Copyright 2009 Andreas Gölzer

This file is part of readESM.

readESM is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

readESM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with readESM.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef TECHNICAL_H
#define TECHNICAL_H TECHNICAL_H
#include "formatStrings.h"
#include "vublock.h"

///See page 163 of l207.pdf
class Technical : public vublock {
	public:
	string name() const{
		return "Technical Data";
	}
	static const int TREP = 0x5;
	Technical(iter nstart) : vublock(nstart) { Init(); }
	int size() const{
		return 2 + 136 + 167*IntByte(136) + 1 + 128;;
	}
	void CompleteReport(reporter& report) const{
		runningIndex = 0;
		report("vuManufacturerMame",fixedString(36) );
		report("vuManufacturerAddress",fixedString(36) );
		report("vuPartNumber",fixedString(16) );
		report("vuSerialNumber",fixedString(8) );
		report("vuSoftwareVersion",fixedString(4) );
		report("vuSoftInstallationDate",readDate().str() );
		report("vuManufacturingDate",readDate().str() );
		report("vuApprovalNumber",fixedString(8) );
		report("sensorSerialNumber",fixedString(8) );
		report("sensorApprovalNumber",fixedString(8) );
		report("sensorPairingDateFirst",readDate().str() );
		
		for(reporter::subblock b = report.newsub("Calibrations", IntByte()); b(); ++b){
			report("calibrationPurpose", formatCalibrationPurpose(IntByte()));
			report("workshopName", fixedString(36) );
			report("workshopAddress", fixedString(36) );
			report("workshopCardNumber", fixedString(18) );
			report("workshopCardExpiryDate", readDate().str());
			report("vehicleIdentificationNumber", fixedString(17));
			report("vehicleRegistrationNation", nationNumeric(IntByte()));
			report("vehicleRegistrationNumber", fixedString(14));
			report("wVehicleCharacteristicConstant", Int16());
			report("kConstantOfRecordingEquipment", Int16());
			report("lTyreCircumference", Int16());
			report("tyreSize", fixedString(15));
			report("authorisedSpeed", IntByte());
			report("oldOdometerValue", Odometer());
			report("newOdometerValue", Odometer());
			report("oldTimeValue", readDate().str());
			report("newTimeValue", readDate().str());
			report("nextCalibrationDate", readDate().str());
		}
	}
	void BriefReport(reporter& report) const{
		CompleteReport(report);
	}

};

#endif
