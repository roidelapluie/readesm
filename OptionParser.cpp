#include <QStringList>
#include <QMap>

#include "OptionParser.h"


OptionParser::OptionParser(bool withQt){
	if(withQt){
		possibleFlags << "cmap" << "widgetcount" << "reverse";
		possibleOptions << "style" << "stylesheet" << "session" << "graphicssystem" << "qmljsdebugger" << "display" << "geometry" << "fn" << "font" << "bg" << "background"  << "fg" << "foreground" << "btn" << "button" << "name" << "title" << "visual" << "ncols" << "im" << "inputstyle";
	}
	addFlag("help","h",tr("Show help text"));
}

void OptionParser::addOption(const QString& longName, const QString& shortName, const QString& description){
	optionAliases[shortName] = longName;
	optionAliases[longName] = shortName;
	possibleOptions << longName << shortName;
	optionDescriptions << ("--" + longName + " | -" + shortName + ": " + description);
}

void OptionParser::addFlag(const QString& longName, const QString& shortName, const QString& description){
	optionAliases[shortName] = longName;
	optionAliases[longName] = shortName;
	possibleFlags << longName << shortName;
	flagDescriptions << ("--" + longName + " | -" + shortName + ": " + description);
}

bool OptionParser::parseArgs(int argc, char* argv[]){
	QStringList cmdArgs;
	for(int j = 1; j < argc; ++j) cmdArgs << argv[j];
	QString optionName = "";
	for(int j = 0; j < cmdArgs.size(); ++j){
		if(optionName != ""){
			options[optionName] = cmdArgs[j];
			optionName = "";
		} else if(cmdArgs[j][0] == '-' && cmdArgs[j].size() > 1){
			int start = 1;
			if(cmdArgs[j][1] == '-') start = 2;
			optionName = cmdArgs[j].mid(start);
			if(possibleFlags.contains(optionName)){
				flags << optionName;
				optionName = "";
			}
		} else arguments << cmdArgs[j];
	}
	return true;
	flagDescriptions.sort();
	optionDescriptions.sort();
}

QString OptionParser::get(const QString& argument, const QString& elsevalue) const{
	if(options.contains(argument)) return options[argument];
	if(options.contains(optionAliases[argument])) return options[optionAliases[argument]];
	return elsevalue;
}

QString OptionParser::getArgument(int j) const{
	return arguments[j];
}

bool OptionParser::checkFlag(const QString& argument) const{
	return flags.contains(argument) || (optionAliases.contains(argument) && options.contains(optionAliases[argument]));
}

int OptionParser::argumentCount() const{
	return arguments.size();
}

QString OptionParser::helpText() const{
	return tr("Possible Flags:") + "\n\n  " + flagDescriptions.join("\n  ") + "\n\n" + tr("Possible Options:") + "\n\n  " + optionDescriptions.join("\n  ");
}