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

#ifndef TECHNICAL_H
#define TECHNICAL_H TECHNICAL_H
#include "formatStrings.h"
#include "vuBlock.h"
#include "constDataPointer.h"

///See page 163 of l207.pdf
class vuTechnical : public vuBlock {
	Q_DECLARE_TR_FUNCTIONS(vuSpeeds)
	public:
	QString name() const {
		return tr("Technical Data");
	}
	static const int TREP = 0x5;
	vuTechnical(constDataPointer nstart) :
		vuBlock(nstart) {
		Init();
	}
	int size() const {
		return 2 + 136 + 167* IntByte (136) + 1 + 128;;
	}
	void CompleteReport(reporter& report) const {
		runningIndex = 0;
		report(tr("vuManufacturerMame"), fixedString(36));
		report(tr("vuManufacturerAddress"), fixedString(36));
		report(tr("vuPartNumber"), fixedString(16));
		report(tr("vuSerialNumber"), hexCode(8));
		report(tr("vuSoftwareVersion"), fixedString(4));
		report(tr("vuSoftInstallationDate"), readDate().str());
		report(tr("vuManufacturingDate"), readDate().str());
		report(tr("vuApprovalNumber"), fixedString(8));
		report(tr("sensorSerialNumber"), hexCode(8));
		report(tr("sensorApprovalNumber"), fixedString(8));
		report(tr("sensorPairingDateFirst"), readDate().str());

		for(reporter::subblock b = report.newsub(tr("VuCalibrationRecord"),
				IntByte()); b(); ++b) {
			report(tr("calibrationPurpose"), formatStrings::calibrationPurpose(IntByte()));
			report(tr("workshopName"), fixedString(36));
			report(tr("workshopAddress"), fixedString(36));
			report(tr("workshopCardNumber"), fixedString(18));
			report(tr("workshopCardExpiryDate"), readDate().str());
			report(tr("vehicleIdentificationNumber"), fixedString(17));
			report(tr("vehicleRegistrationNation"), formatStrings::nationNumeric(IntByte()));
			report(tr("vehicleRegistrationNumber"), fixedString(14));
			report(tr("wVehicleCharacteristicConstant"), stringify(Int16()) + " "
					+ tr("Impulses") + "/km");
			report(tr("kConstantOfRecordingEquipment"), stringify(Int16()) + " "
					+ tr("Impulses") + "/km");
			int lTyreCircumference = Int16();
			report(tr("lTyreCircumference"), stringify(lTyreCircumference / 8)
					+ " " + (lTyreCircumference % 8 ? stringify(
					lTyreCircumference % 8) + "/8 " : "") + "mm");
			report(tr("tyreSize"), fixedString(15));
			report(tr("authorisedSpeed"), stringify(IntByte()) + " km/h");
			report(tr("oldOdometerValue"), stringify(Odometer()) + " km");
			report(tr("newOdometerValue"), stringify(Odometer()) + " km");
			report(tr("oldTimeValue"), readDate().str());
			report(tr("newTimeValue"), readDate().str());
			report(tr("nextCalibrationDate"), readDate().str());
		}
	}
	void BriefReport(reporter& report) const {
		CompleteReport(report);
	}

};

#endif
