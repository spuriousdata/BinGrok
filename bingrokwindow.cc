#include "bingrokwindow.h"
#include "ui_bingrokwindow.h"
#include "hexwidget.h"
#include "preferences.h"
#include "unistd.h"

#include <QSettings>
#include <QCloseEvent>
#include <QString>
#include <QFileDialog>
#include <QDataStream>
#include <QMessageBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QDialogButtonBox>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

BinGrokWindow::BinGrokWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::BinGrokWindow),
	preferences_ui(NULL)
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
	connect(ui->action_Preferences, SIGNAL(triggered()),
			this, SLOT(show_preferences()));

	connect(hexwidget, SIGNAL(update_scroll(off_t,off_t)),
			this, SLOT(update_scroll(off_t,off_t)));

	read_settings();
}

void BinGrokWindow::read_settings()
{
	QSettings s;
#ifndef QT_NO_DEBUG
	qDebug() << "s.childGroups() = " << s.childGroups();
#endif
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
#ifndef QT_NO_DEBUG
	qDebug("save() called");
#endif
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

void BinGrokWindow::show_preferences()
{
	if (preferences_ui == NULL) {
		preferences_ui = new Preferences(hexwidget);
		connect(preferences_ui, SIGNAL(preferences_changed()),
				this, SLOT(save_preferences()));
	}
	preferences_ui->show();
}

void BinGrokWindow::save_preferences()
{
	// if we had higher level preferences, we could save them here
	hexwidget->update_preferences(
					preferences_ui->get_bpc(),
					preferences_ui->get_font()
				);
}

void BinGrokWindow::update_scroll(off_t s, off_t e)
{
#ifndef QT_NO_DEBUG
	qDebug() << "update_scroll(" << s << ", " << e << ")";
#endif
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
	delete preferences_ui;
    delete ui;
}
