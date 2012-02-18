#ifndef PICTUREGENERATOR_H
#define PICTUREGENERATOR_H PICTUREGENERATOR_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QTextStream>


class PictureGenerator {
public:
	///returns the string to be used in the document
	virtual QString toString() const;

	///reset to the initial state
	virtual void reset();

protected:
	PictureGenerator();
	QString collected;
	QTextStream collector;
	bool finished;
};
#endif
