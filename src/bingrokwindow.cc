#include "bingrokwindow.h"
#include "ui_bingrokwindow.h"
#include "hexwidget.h"
#include "txtdisplaywidget.h"
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
#include <QAction>
#include <QAbstractSlider>

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
    QSizePolicy p1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    p1.setHorizontalStretch(3); // stretch factor 3 (one byte takes up 3 columns including it's space)
    hexwidget->setSizePolicy(p1);
	layout->addWidget(hexwidget);

    txtdisplaywidget = new TxtDisplayWidget(container);
    QSizePolicy p2(QSizePolicy::Preferred, QSizePolicy::Preferred);
    p2.setHorizontalStretch(1); // stretch factor 1 (one byte takes up 1 column -- no spaces)
    txtdisplaywidget->setSizePolicy(p2);

    layout->addWidget(txtdisplaywidget);
    hexwidget->set_txtdisplay(txtdisplaywidget);

	vscroll = new QScrollBar(container);
	layout->addWidget(vscroll);
	vscroll->setRange(0,0);
	vscroll->setTracking(true);

	hexwidget->set_scrollbar(vscroll);

	setCentralWidget(container);

	connect(ui->action_New, SIGNAL(triggered()), this, SLOT(new_file()));
	connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(save_as()));
	connect(ui->action_Preferences, SIGNAL(triggered()),
			this, SLOT(show_preferences()));

    /*connect(hexwidget, SIGNAL(update_scroll(off_t,off_t)),
            this, SLOT(update_scroll(off_t,off_t)));*/
	connect(hexwidget, SIGNAL(file_opened(QFile*)),
			this, SLOT(add_recently_open(QFile*)));

	read_settings();
	redraw_recently_open();
}

void BinGrokWindow::read_settings()
{
	QSettings s;

	s.beginGroup("window");
	resize(s.value("size", QSize(400,400)).toSize());
	move(s.value("position", QPoint(200,200)).toPoint());
	max_recently_open = s.value("max_recently_open", 8).toInt();

	recently_open = s.value("recently_open").toStringList();

	s.endGroup();
}

void BinGrokWindow::open()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty())
		hexwidget->open(filename);
}

void BinGrokWindow::open_file(const QString &f)
{
	hexwidget->open(f);
}

void BinGrokWindow::open_recent()
{
	QAction *a = qobject_cast<QAction *>(sender());
	if (a) hexwidget->open(a->data().toString());
}

void BinGrokWindow::new_file()
{
	qDebug("new_file() called");
	hexwidget->new_file();
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
		preferences_ui->set_mro(max_recently_open);

		connect(preferences_ui, SIGNAL(preferences_changed()),
				this, SLOT(save_preferences()));
	}
	preferences_ui->show();
}

void BinGrokWindow::save_preferences()
{
	max_recently_open = preferences_ui->get_mro();

	hexwidget->update_preferences(
					preferences_ui->get_bpc(),
					preferences_ui->get_fontsize()
				);
}

void BinGrokWindow::add_recently_open(QFile *f)
{
	if (recently_open.contains(f->fileName())){
		QString fname = recently_open.takeAt(
					recently_open.indexOf(f->fileName())
					);
		recently_open.prepend(fname);
	} else {
		recently_open.prepend(f->fileName());
		while (recently_open.size() > max_recently_open) {
			recently_open.removeLast();
		}
	}
	redraw_recently_open();
}

void BinGrokWindow::redraw_recently_open()
{
	ui->menuOpen_Recent->clear();
	for (int i = 0; i < recently_open.size(); i++) {
		QString txt = QString("&%1 %2").arg(i+1).arg(recently_open[i]);
		QAction *action = ui->menuOpen_Recent->addAction(txt,
									   this, SLOT(open_recent()));
		action->setData(recently_open[i]);
		action->setVisible(true);
	}
}

void BinGrokWindow::closeEvent(QCloseEvent *e)
{
	if (hexwidget->maybe_save()) {
		/* Save size/position on close */
		QSettings s;

		s.beginGroup("window");
		s.setValue("size", size());
		s.setValue("position", pos());

		/*
		s.setValue("recently_open_size", recently_open.size());
		s.beginWriteArray("recently_open");
		for (int i = 0; i < recently_open.size(); i++) {
			s.setArrayIndex(i);
			s.setValue("file", recently_open[i]);
		}
		s.endArray();
		*/
		s.setValue("recently_open", recently_open);
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
