#include "bingrokwindow.h"
#include "ui_bingrokwindow.h"

#include <QSettings>
#include <QCloseEvent>
#include <QString>
#include <QFileDialog>
#include <QDataStream>
#include <QMessageBox>
#include <QDebug>

BinGrokWindow::BinGrokWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::BinGrokWindow)
{
    ui->setupUi(this);

	// out slots
	connect(ui->action_New, SIGNAL(triggered()), this, SLOT(new_file()));
	connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(save_as()));

	// built-in slots
	//connect(ui->action_Cut, SIGNAL(triggered()),

	read_settings();
}

void BinGrokWindow::read_settings()
{
	QSettings s;

	s.beginGroup("bgwindow");
	resize(s.value("size", QSize(400,400)).toSize());
	move(s.value("position", QPoint(200,200)).toPoint());
	s.endGroup();
}

void BinGrokWindow::open()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty()) {
		// load file into bghexwidget
		qDebug() << "Called Open(" << filename << ")";
	}
}

void BinGrokWindow::new_file()
{
	qDebug("new_file() called");
	// clear bghexwidget
}

bool BinGrokWindow::save()
{
	qDebug("save() called");
	/*
	  if bghexwidget contents is empty:
		return saveAs();
	  else
		return save_file(current file);
	*/
	return true;
}

bool BinGrokWindow::save_as()
{
	QString filename = QFileDialog::getSaveFileName(this);
	if (filename.isEmpty())
		return false;
	return save_file(filename);
}

bool BinGrokWindow::save_file(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("BinGrok"),
							 tr("Cannot write file %1:\n%2.")
							 .arg(filename).arg(file.errorString())
							 );
		return false;
	}
	QDataStream out(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	//out << bghexwidget content
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void BinGrokWindow::closeEvent(QCloseEvent *e)
{
	/* Save size/position on close */
	QSettings s;
	s.beginGroup("bgwindow");
	s.setValue("size", size());
	s.setValue("position", pos());
	s.endGroup();

	e->accept();
}

BinGrokWindow::~BinGrokWindow()
{	
    delete ui;
}
