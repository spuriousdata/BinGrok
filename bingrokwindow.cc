#include "bingrokwindow.h"
#include "ui_bingrokwindow.h"
#include "hexwidget.h"
#include "unistd.h"

#include <QSettings>
#include <QCloseEvent>
#include <QString>
#include <QFileDialog>
#include <QDataStream>
#include <QMessageBox>
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>
#include <QScrollBar>

BinGrokWindow::BinGrokWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::BinGrokWindow)
{
	container = new QWidget(this);
	layout = new QHBoxLayout(container);

    ui->setupUi(this);

	hexwidget = new HexWidget(container);
	hexwidget->setSizePolicy(QSizePolicy(
								 QSizePolicy::Expanding,
								 QSizePolicy::Expanding
								 )
							 );
	layout->addWidget(hexwidget);
	vscroll = new QScrollBar(container);
	layout->addWidget(vscroll);
	vscroll->setRange(0,0);
	setCentralWidget(container);

	connect(ui->action_New, SIGNAL(triggered()), this, SLOT(new_file()));
	connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(save_as()));

	connect(hexwidget, SIGNAL(update_scroll(off_t,off_t)),
			this, SLOT(update_scroll(off_t,off_t)));

	read_settings();
}

void BinGrokWindow::read_settings()
{
	QSettings s;

	qDebug() << s.childGroups();

	s.beginGroup("window");
	QSize size = s.value("size", QSize(400,400)).toSize();
	QPoint pos = s.value("position", QPoint(200,200)).toPoint();
	resize(size);
	move(pos);
	s.endGroup();
}

void BinGrokWindow::open()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty()) {
		hexwidget->open(filename);
	}
}

void BinGrokWindow::new_file()
{
	qDebug("new_file() called");
	// clear bghexwidget
}

void BinGrokWindow::save()
{
	qDebug("save() called");
	/*
	  if bghexwidget contents is empty:
		return saveAs();
	  else
		return save_file(current file);
	*/
}

void BinGrokWindow::save_as()
{
	QString filename = QFileDialog::getSaveFileName(this);
	if (filename.isEmpty())
		return;
	save_file(filename);
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

void BinGrokWindow::update_scroll(off_t s, off_t e)
{
	qDebug() << "update_scroll(" << s << ", " << e << ")";
	vscroll->setRange(s, e);
}

void BinGrokWindow::closeEvent(QCloseEvent *e)
{
	if (hexwidget->maybe_save()) {
		/* Save size/position on close */
		QSettings s;

		s.beginGroup("window");
		s.setValue("size", size());
		s.setValue("position", pos());
		s.endGroup();
		s.sync();

		e->accept();
	} else {
		e->ignore();
	}
}

BinGrokWindow::~BinGrokWindow()
{
	delete vscroll;
	delete hexwidget;
    delete ui;
}
