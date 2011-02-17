#include <QtGui/QApplication>
#include "gui/mainWindow.h"

#include "legacyWrapper.h"

int main(int argc, char** argv)
{
	initTranslation();
	QApplication app(argc, argv);
	mainWindow widgetMainWindow;
	widgetMainWindow.show();
	if(argc > 1){
		widgetMainWindow.openFile(QString(argv[1]));
	}
	return app.exec();
}
