#include "EsmFile.h"

#include "config.h"
#include "reporter/reporter.h"
#include "blockFactory.h"

#include <QtCore/QFile>
#include <QtCore/QObject>

reporter& operator<<(reporter& report, const EsmFile& e) {
	e.printOn(report);
// 	report.title = e.name();
// 	report.bigblockstart(QObject::tr("Statistics"));
// 	report.single(QObject::tr("Statistics for") + " " + e.title, true);
// 	report.single(formatRange(e.first, e.last));
// 	report(QObject::tr("Recorded days"), e.daycount);
// 	report(QObject::tr("Overall driven distance"), QString("%1 km").arg(e.drivenkm));
// 	report(QObject::tr("Overall driving time"), formatMinutes(e.drivenminutes));
// 	if(e.daycount)
// 	{
// 		report(QObject::tr("Average distance per day"), QString("%1 km").arg(e.drivenkm / e.daycount));
// 		report(QObject::tr("Average time driven per day"), formatMinutes(e.drivenminutes
// 				/ e.daycount) + " (" + stringify(100* e .drivenminutes
// 				/ (e.daycount * 24* 60 ))+ "%)");
// 	}
// 	if(e.drivenminutes)
// 		report(QObject::tr("Average speed when driving"), stringify(e.drivenkm * 60 / e.drivenminutes) + " km/h");
// 	report.bigblockend();
// 
// 	for(esmfile::subiter i = e.blocks.begin(); i < e.blocks.end(); ++i) report << **i;
// 
// #ifndef HAVE_NO_CRYPTO
// 	report.bigblockstart(QObject::tr("Key chain"));
// 	report << *e.CAcert;
// 	report << *e.devicecert;
// 	report.bigblockend();
// #endif
	return report;

}
void EsmFile::printOn(reporter& report) const{
	for(int j = 0; j < blocks.size(); ++j) {
		report << *(blocks[j]);
	}
}

EsmFile::EsmFile(const QString& filename) : fileWalker(constDataPointer::loadFile(filename)){
	while(fileWalker.bytesLeft() > 0) {
		QSharedPointer<Block> p(blockFactory(fileWalker));
		blocks.append(p);
		fileWalker += p->size();
	}

/*#ifndef HAVE_NO_CRYPTO
	if(CAcert && devicecert) {
		if(CAcert->verify(":/EC_PK.bin")) {
			if(devicecert->verify(*CAcert)) {
				for(subiter i = blocks.begin(); i < blocks.end(); ++i)
					(*i)->checksig(devicecert->key);
			}
		} else {
			qDebug() << "Cannot verify certificates and signatures: European main certificate file not found or not openable.";
		}
	}
#endif*/
}

template <typename base, typename derived>
QSharedPointer<derived> findTypeInVector(QVector< QSharedPointer<base> > array){
	QSharedPointer<derived> pointer;
	for(int j = 0; j < array.size(); ++j){
		pointer = qSharedPointerDynamicCast<derived>(array[j]);
		if(!pointer.isNull()) return pointer;
	}
	return pointer;
}

QString EsmFile::suggestTitle() const {
	QSharedPointer<VuOverview> ov = findTypeInVector<Block, VuOverview>(blocks);
	if(!ov.isNull()) return ov->vehicleRegistrationIdentification.vehicleRegistrationNumber;
	return "bla";
	/*return tr("%1, %2 to %3")
		.arg(title)
		.arg(first.datestr())
		.arg(last.datestr());*/
}

QString EsmFile::suggestFileName() const {
	return "bla";
/*	return tr("%1  (%2 to %3)")
		.arg(title)
		.arg(first.datestr("%F"))
		.arg(last.datestr("%F"));*/
}
