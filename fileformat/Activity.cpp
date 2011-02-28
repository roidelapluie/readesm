#include "Activity.h"

QString visualization(reporter& o, const std::vector<Activity>& acts) {
	reporter::pgptr actvisual(o.getBarGraph());
	for(std::vector<Activity>::const_iterator j(acts.begin()); j != acts.end(); ++j) {
		if(j->duration > 10000) qDebug() << "ouch, Duration = " << j->duration;
		QString descr = j->astr() + " " + QObject::tr("for") + " "
				+ Activity::formatDurTime(j->duration) + " " + QObject::tr("from") + " "
				+ j->tstr() + " " + QObject::tr("to") + " " + j->tstr(j->duration);
		int height = 10;
		QString color;
		switch(j->activity) {
			case Activity::Work:
				height = 70;
				color = "yellow";
				break;
			case Activity::Available:
				height = 10;
				color = "black";
				break;
			case Activity::Driving:
				height = 100;
				color = "green";
				break;
			case Activity::Break:
				if(j->duration >= 15) {
					height = 40;
					color = "red";
				} else {
					height = 50;
					color = "blue";
				}
				break;
			default:
				actvisual->collector << (j->activity) << (j->str()) << descr;
		}
		actvisual->add(j->time, j->duration, height, color, descr);
	}
	return actvisual->str();
}
