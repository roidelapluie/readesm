#include "mainWindow.h"

#include "../fileformat/esmfile.h"
#include "../fileformat/reporter/xmlReporter.h"

#include <QtCore/QString>
#include <QtCore/QStringBuilder>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>

mainWindow::mainWindow()
{
	view = new QWebView(this);
	view->load(QUrl("qrc:///greeting.xhtml"));
	setCentralWidget(view);
	
	//define and link the stuff in the menu bar
	QMenu *fileMenu = new QMenu(tr("&File"),this); 
	QAction* fileOpenAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open…"), this);
	QAction* fileSaveRawAction = new QAction(QIcon::fromTheme("document-save-as"), tr("Save &As…"), this);
	QAction* fileSaveHtmlAction = new QAction(tr("E&xport as HTML…"), this);
	QAction* filePrintAction = new QAction(QIcon::fromTheme("document-print"), tr("&Print…"), this);
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
		qApp->applicationName() % QString("\n\n") 
		% tr("Version: ") % qApp->applicationVersion()
		% QString("\n")
		% tr("Copyright (C) 2011 by %1 (%2)")
			.arg(qApp->organizationName())
			.arg(qApp->organizationDomain())
		% QString("\n\n") 
		% tr("This program converts digital tachograph files into human-readable form.")
		% QString("\n\n")
		% tr("readesm is free software: you can redistribute it and/or modify it under the "
		"terms of the GNU General Public License as published by the Free Software Foundation, "
		"either version 3 of the License, or (at your option) any later version.\n\n"
		"readesm is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; "
		"without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR "
		"PURPOSE. See the GNU General Public License for more details.\n\nYou should have "
		"received a copy of the GNU General Public License along with readesm.  If not, "
		"see <http://www.gnu.org/licenses/>.")
	);
}

void mainWindow::helpContents()
{
	view->load(QUrl("qrc:///homepage/help.html"));
	view->load(QUrl("bla.xhtml"));
	//view->page()->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );
}

void mainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Tachograph File"), 
		QString(), 
		tr("Tachograph Files") +  "(*.esm *.ddd *.tgd)" + ";;" + tr("All files") + "(*)"
	);
        if(fileName != "") openFile(fileName);
}

void mainWindow::openFile(const QString& filename)
{
	esm = QSharedPointer<esmfile>(new esmfile(filename));
	xmlReporter rep;
	rep << *esm;
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
	if(!esm) {
		QMessageBox::warning(this, tr("Saving not possible"),tr("Nothing opened, nothing to save."));
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save XHtml file as"), 
		esm->suggestFileName() + ".xhtml", 
		tr("XHtml files") +  "(*.xhtml)" + ";;" + tr("All files") + "(*)"
	);
	if(fileName != "") {
		QString content = view->page()->mainFrame()->toHtml();
		QFile file(fileName);
		if(!file.open(QIODevice::WriteOnly)) {
			QMessageBox::warning(this, tr("Saving not possible"),tr("Could not open file."));
			return;
		}
		file.write(content.toUtf8());
	}
}

void mainWindow::saveRaw()
{
	if(!esm) {
		QMessageBox::warning(this, tr("Saving not possible"),tr("Nothing opened, nothing to save."));
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Tachograph file as"), 
		esm->suggestFileName() + ".esm", 
		tr("Tachograph Files") +  "(*.esm *.ddd)" + ";;" + tr("All files") + "(*)"
	);
	if(fileName != "") {
		QFile file(fileName);
		if(!file.open(QIODevice::WriteOnly)) {
			QMessageBox::warning(this, tr("Saving not possible"),tr("Could not open file."));
			return;
		}
		file.write(esm->content);
	}
}

#include "mainWindow.moc"
