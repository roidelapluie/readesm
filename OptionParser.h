#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H OPTIONPARSER_H

#include <QCoreApplication>
#include <QMap>
#include <QStringList>
#include <QString>

class OptionParser {
	Q_DECLARE_TR_FUNCTIONS(OptionParser)
protected:
	QStringList arguments;
	QStringList possibleFlags;
	QStringList possibleOptions;
	QStringList flags;
	QStringList flagDescriptions;
	QStringList optionDescriptions;

	QMap<QString, QString> options;
	QMap<QString, QString> optionAliases;
public:
	OptionParser(bool withQt = true);
	void addOption(const QString& longName, const QString& shortName, const QString& description);
	void addFlag(const QString& longName, const QString& shortName, const QString& description);
	bool parseArgs(int argc, char* argv[]);
	QString get(const QString& argument, const QString& elsevalue = "") const;
	bool checkFlag(const QString& argument) const;
	QString getArgument(int j) const;
	int argumentCount() const;
	QString helpText() const;
};

#endif
