#ifndef mainWindow_H
#define mainWindow_H

#include <QMainWindow>
#include <QString>
class QWebView;


class mainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		QWebView* view;
		mainWindow();

	public slots:
		void openFile();
		void openFile(const QString& filename);
		void helpOnline();
		void print();
		void saveRaw();
		void saveHtml();
		void helpAbout();
	
};

#endif
