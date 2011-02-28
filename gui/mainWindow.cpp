#include "mainWindow.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>

#include <QPrinter>
#include <QPrintDialog>
#include <QtWebKit>

#include "../fileformat/esmfile.h"
#include "../fileformat/reporter/xmlReporter.h"

mainWindow::mainWindow()
{
	view = new QWebView(this);
	view->setHtml("readesm");
	setCentralWidget(view);
	
	//define and link the stuff in the menu bar
	QMenu *fileMenu = new QMenu(tr("&File"),this); 
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
	
	QMenu* helpMenu = new QMenu(tr("&Help"),this); 
	QAction* helpContentsAction = new QAction(QIcon::fromTheme("help-contents"), tr("&Contents"), this);
	QAction* helpAboutAction = new QAction(QIcon::fromTheme("help-about"), tr("&About Readesm"), this);
	QAction* helpAboutQtAction = new QAction(tr("About &Qt"), this);
	connect(helpContentsAction, SIGNAL(triggered()), SLOT(helpContents()) );
	connect(helpAboutAction, SIGNAL(triggered()), SLOT(helpAbout()) );
	connect(helpAboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
	helpContentsAction->setShortcut(QKeySequence::HelpContents);
	helpMenu->addAction(helpContentsAction);
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

void mainWindow::helpContents()
{
	view->load(QUrl("qrc:///homepage/help.html"));
}

void mainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Tachograph File"), 
		QString(), 
		tr("Tachograph Files") +  "(*.esm *.ddd)" + ";;" + tr("All files") + "(*)"
	);
	openFile(fileName);
}

void mainWindow::openFile(const QString& filename)
{
	esmfile esm(filename);
	xmlReporter rep;
	rep << esm;
	view->setContent(rep.str().toUtf8(), "application/xhtml+xml");
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
	QString content = view->page()->mainFrame()->toHtml();
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save XHtml file as"), 
		QString(), 
		tr("XHtml files") +  "(*.xhtml)"
	);
	
}

void mainWindow::saveRaw()
{

}

QSize mainWindow::sizeHint() const
{
    return QSize(800,400);
}

#include "mainWindow.moc"
