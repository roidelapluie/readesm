#ifndef mainWindow_H
#define mainWindow_H

#include <QtCore/QSharedPointer>
#include <QtGui/QMainWindow>

class esmfile;
class QString;
class QWebView;

class mainWindow : public QMainWindow
{
	Q_OBJECT
	private:
		QSharedPointer<esmfile> esm;
	public:
		QWebView* view;
		mainWindow();
	public slots:
		void openFile();
		void openFile(const QString& filename);
		void helpContents();
		void print();
		void saveRaw();
		void saveHtml();
		void helpAbout();
};

#endif
