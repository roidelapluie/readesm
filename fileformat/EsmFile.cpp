#include "EsmFile.h"

#include "config.h"

#include "blockFactory.h"
#include "CardBlocks/CardCertificate.h"
#include "CardBlocks/Identification.h"
#include "CardBlocks/MemberStateCertificate.h"
#include "CardBlocks/CardDriverActivity.h"

#include "DataTypes/EncryptedCertificate.h"
#include "Reporter/Reporter.h"
#include "VuBlocks/VuOverview.h"
#include "VuBlocks/VuActivities.h"
#include "loadFile.h"

#include <QtCore/QFile>
#include <QtCore/QObject>

template <typename base, typename derived>
QSharedPointer<derived> findTypeInVector(QVector< QSharedPointer<base> > array){
	QSharedPointer<derived> pointer;
	for(int j = 0; j < array.size(); ++j){
		pointer = qSharedPointerDynamicCast<derived>(array[j]);
		if(!pointer.isNull()) return pointer;
	}
	return pointer;
}

template <typename base, typename derived>
QVector< QSharedPointer<derived> > findManyInVector(QVector< QSharedPointer<base> > array){
	QVector< QSharedPointer<derived> > rv;
	QSharedPointer<derived> pointer;
	for(int j = 0; j < array.size(); ++j){
		pointer = qSharedPointerDynamicCast<derived>(array[j]);
		if(!pointer.isNull()) rv.append(pointer);
	}
	return rv;
}

Reporter& operator<<(Reporter& report, const EsmFile& e) {
	e.printOn(report);
	report.setTitle(e.suggestTitle());
	return report;
}

void EsmFile::printOn(Reporter& report) const{
	for(int j = 0; j < blocks.size(); ++j) {
		report.writeBlock(*blocks[j]);
	}
}

EsmFile::EsmFile(const QString& filename) : fileData(loadFile(filename)), errors() {
	if(fileData.size() == 0) errors = tr("Could not read any data from file '%1'.").arg(filename);
	DataPointer fileWalker(fileData);
	while(fileWalker.bytesLeft() > 0) {
		QSharedPointer<TopLevelBlock> p(blockFactory(fileWalker));
		blocks.append(p);
		fileWalker += p->size();
	}
#ifdef HAVE_CRYPTO
	PlainCertificate ercaKey(DataPointer(loadFile(":/EC_PK.bin")));
	EncryptedCertificate* caCertificate = NULL;
	EncryptedCertificate* deviceCertificate = NULL;
	QSharedPointer<VuOverview> ov = findTypeInVector<TopLevelBlock, VuOverview>(blocks);
	if(ov){
		caCertificate = &ov->memberStateCertificate;
		deviceCertificate = &ov->vuCertificate;
	} else {
		QSharedPointer<MemberStateCertificate> mc = findTypeInVector<TopLevelBlock, MemberStateCertificate>(blocks);
		if(mc) caCertificate = &mc->certificate;
		QSharedPointer<CardCertificate> dc = findTypeInVector<TopLevelBlock, CardCertificate>(blocks);
		if(dc) deviceCertificate = &dc->certificate;
	}
	if(caCertificate) caCertificate->attemptVerificationFrom(ercaKey);
	if(deviceCertificate) deviceCertificate->attemptVerificationFrom(*caCertificate);
	if(deviceCertificate && deviceCertificate->isVerified()){
		for(int j = 0; j < blocks.size(); ++j) {
			blocks[j]->checkSignature(*deviceCertificate);
		}
	}
#endif
}

QString EsmFile::suggestTitle() const {
	QSharedPointer<VuOverview> ov = findTypeInVector<TopLevelBlock, VuOverview>(blocks);
	QSharedPointer<Identification> id = findTypeInVector<TopLevelBlock, Identification>(blocks);
	QSharedPointer<CardDriverActivity> act = findTypeInVector<TopLevelBlock, CardDriverActivity>(blocks);
	QString rv("Esm data");
	QString dateSuggestion;
	if(!ov.isNull()){
		rv = ov->vehicleRegistrationIdentification.vehicleRegistrationNumber;
		dateSuggestion = ov->currentDateTime.date().toString();
	}
	if(!id.isNull()) rv = id->cardHolderName.toString();
	QDate minDate, maxDate;
	if(!act.isNull()){
		if(act->cardActivityDailyRecords.numberOfBlocks() > 1){
			dateSuggestion = tr("%1 to %2").arg(act->cardActivityDailyRecords[0].activityRecordDate.date().toString(), act->cardActivityDailyRecords[act->cardActivityDailyRecords.numberOfBlocks() - 1].activityRecordDate.date().toString());
		}
	}
	if(dateSuggestion != "") rv += " " + dateSuggestion;
	return rv;
}

QString EsmFile::suggestFileName() const {
	return suggestTitle();
}

QString EsmFile::errorLog() const {
	return errors;
}
