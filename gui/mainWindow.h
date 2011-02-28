#ifndef mainWindow_H
#define mainWindow_H

#include <QMainWindow>
#include <QString>
#include <QSize>
class QWebView;


class mainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		QWebView* view;
		mainWindow();
		virtual QSize sizeHint () const;
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
