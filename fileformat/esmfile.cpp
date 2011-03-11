#include "esmfile.h"

#include "config.h"
#include "reporter/reporter.h"

#include <QtCore/QDebug>
#include <QtCore/QObject>

reporter& operator<<(reporter& report, const esmfile& e) {
	report.title = e.name();
	report.bigblockstart(QObject::tr("Statistics"));
	report.single(QObject::tr("Statistics for") + " " + e.title, true);
	report.single(formatRange(e.first, e.last));
	report(QObject::tr("Recorded days"), e.daycount);
	report(QObject::tr("Overall driven distance"), QString("%1 km").arg(e.drivenkm));
	report(QObject::tr("Overall driving time"), formatMinutes(e.drivenminutes));
	if(e.daycount)
	{
		report(QObject::tr("Average distance per day"), QString("%1 km").arg(e.drivenkm / e.daycount));
		report(QObject::tr("Average time driven per day"), formatMinutes(e.drivenminutes
				/ e.daycount) + " (" + stringify(100* e .drivenminutes
				/ (e.daycount * 24* 60 ))+ "%)");
	}
	if(e.drivenminutes)
		report(QObject::tr("Average speed when driving"), stringify(e.drivenkm * 60 / e.drivenminutes) + " km/h");
	report.bigblockend();

	for(esmfile::subiter i = e.blocks.begin(); i < e.blocks.end(); ++i) report << **i;
	return report;

}

esmfile::esmfile(const QString& filename) :
	esmfilehead(filename) 
{
	constDataPointer filewalker(content, 0);
	while(filewalker.bytesLeft() > 0) {
		block::ptr p(block::Factory(filewalker));
		blocks.push_back(p);
		filewalker += p->size();
	}
	for(subiter i = blocks.begin(); i < blocks.end(); ++i)
		(*i)->reportstuff(*this);
#ifndef HAVE_NO_CRYPTO
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
#endif
}

QString esmfile::name() const {
	return tr("%1, %2 to %3")
		.arg(title)
		.arg(first.datestr())
		.arg(last.datestr());
}

QString esmfile::suggestFileName() const {
	return tr("%1  (%2 to %3)")
		.arg(title)
		.arg(first.datestr("%F"))
		.arg(last.datestr("%F"));
}
