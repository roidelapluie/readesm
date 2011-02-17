#include "mainWindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>

#include <QtWebKit>

#include "../legacyWrapper.h"

mainWindow::mainWindow() {
	
	view = new QWebView(this);
	//view->setHtml("All art is quite useless.");
	view->load(QUrl("file:///home/goelzera/Programme/readesm/build/last.xml#7"));
	setCentralWidget(view);
	
	//define and link the stuff in the menu bar
	QMenu *fileMenu = new QMenu("&File",this); 
	QAction* fileOpenAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
	QAction* fileQuitAction = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this);
	connect(fileQuitAction, SIGNAL(triggered()), SLOT(close()) );
	connect(fileOpenAction, SIGNAL(triggered()), SLOT(openFile()) );
	fileMenu->addAction(fileOpenAction);
	fileMenu->addSeparator();
	fileMenu->addAction(fileQuitAction);
	menuBar()->addMenu(fileMenu);
	
	QMenu* helpMenu = new QMenu("&Help",this); 
	QAction* helpOnlineAction = new QAction(QIcon::fromTheme("help-contents"), tr("&Online"), this);
	QAction* helpAboutAction = new QAction(QIcon::fromTheme("help-about"), tr("&About"), this);
	QAction* helpAboutQtAction = new QAction(tr("About &Qt"), this);
	connect(helpOnlineAction, SIGNAL(triggered()), SLOT(helpOnline()) );
	connect(helpAboutAction, SIGNAL(triggered()), SLOT(helpAbout()) );
	connect(helpAboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
	helpMenu->addAction(helpOnlineAction);
	helpMenu->addSeparator();
	helpMenu->addAction(helpAboutQtAction);
	helpMenu->addAction(helpAboutAction);
	menuBar()->addMenu(helpMenu);
      
}

void mainWindow::helpAbout()
{
	QMessageBox::about(this,
		tr("About Readesm"),
		tr("This program converts digital tachograph files into human-readable form.")
	);
}

void mainWindow::helpOnline()
{
	view->load(QUrl("http://readesm.sourceforge.net/"));
}

void mainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Tachograph File"), 
		QString(), 
		tr("Tachograph Files (*.esm *.ddd)")
	);
	openFile(fileName);
}

void mainWindow::openFile(const QString& filename)
{
	view->setHtml(QString(convertFile(filename.toStdString()).c_str()));

}

#include "mainWindow.moc"
