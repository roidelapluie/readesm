#include "mainWindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>

#include <QPrinter>
#include <QPrintDialog>
#include <QtWebKit>

#include "../legacyWrapper.h"

mainWindow::mainWindow()
{
	view = new QWebView(this);
	view->setHtml("All art is quite useless.");
	setCentralWidget(view);
	
	//define and link the stuff in the menu bar
	QMenu *fileMenu = new QMenu("&File",this); 
	QAction* fileOpenAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
	QAction* fileSaveRawAction = new QAction(QIcon::fromTheme("document-save-as"), tr("Save &As"), this);
	QAction* fileSaveHtmlAction = new QAction(tr("Save &Html"), this);
	QAction* filePrintAction = new QAction(QIcon::fromTheme("document-print"), tr("&Print"), this);
	QAction* fileQuitAction = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this);
	fileOpenAction->setShortcut(QKeySequence::Open);
	filePrintAction->setShortcut(QKeySequence::Print);
	fileSaveRawAction->setShortcut(QKeySequence::SaveAs);
	fileQuitAction->setShortcut(QKeySequence::Quit);
	connect(fileQuitAction, SIGNAL(triggered()), SLOT(close()) );
	connect(fileOpenAction, SIGNAL(triggered()), SLOT(openFile()) );
	connect(filePrintAction, SIGNAL(triggered()), SLOT(print()) );
	connect(fileSaveRawAction, SIGNAL(triggered()), SLOT(saveRaw()) );
	connect(fileSaveHtmlAction, SIGNAL(triggered()), SLOT(saveHtml()) );
	
	fileMenu->addAction(fileOpenAction);
	fileMenu->addSeparator();
	fileMenu->addAction(fileSaveRawAction);
	fileMenu->addAction(fileSaveHtmlAction);
	fileMenu->addAction(filePrintAction);
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
	helpOnlineAction->setShortcut(QKeySequence::HelpContents);
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
	view->setContent(QByteArray(convertFile(filename.toStdString()).c_str()), "application/xhtml+xml");

}

void mainWindow::print()
{
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print Document"));
	if (dialog->exec() != QDialog::Accepted)
		return;
	view->print(&printer);
}

void mainWindow::saveHtml()
{

}

void mainWindow::saveRaw()
{

}


#include "mainWindow.moc"
