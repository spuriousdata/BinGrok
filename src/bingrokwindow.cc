#include "bingrokwindow.h"
#include "ui_bingrokwindow.h"
#include "hexwidget.h"
#include "txtdisplaywidget.h"
#include "preferences.h"
#include "structeditor.h"
#include "unistd.h"
#include "project.h"

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
#include <QKeySequence>

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

    /* This isn't really a hex "editior" you can't create or edit files, so "New" is a pointless function */
    //connect(ui->action_New, SIGNAL(triggered()), this, SLOT(new_file()));
	connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui->action_SaveAs, SIGNAL(triggered()), this, SLOT(save_as()));
    connect(ui->action_Preferences, SIGNAL(triggered()), this, SLOT(show_preferences()));
    connect(ui->action_Struct_Editor, SIGNAL(triggered()), this, SLOT(show_struct_editor()));

	connect(hexwidget, SIGNAL(file_opened(QFile*)),
			this, SLOT(add_recently_open(QFile*)));

	read_settings();
	redraw_recently_open();

    structeditor_ui = new StructEditor(this);
}

void BinGrokWindow::read_settings()
{
	QSettings s;

	s.beginGroup("window");
	resize(s.value("size", QSize(400,400)).toSize());
	move(s.value("position", QPoint(200,200)).toPoint());
    max_recently_open = static_cast<quint8>(s.value("max_recently_open", 8).toUInt());

	recently_open = s.value("recently_open").toStringList();

	s.endGroup();
}

void BinGrokWindow::open()
{
    QString f = QFileDialog::getOpenFileName(this);
    if (!f.isEmpty())
        open_file(f);
    else
        QMessageBox::warning(this, "Error", "Filename cannot be empty!");
}

void BinGrokWindow::open_file(const QString &f)
{
    QString structdata;
    bool _emit = true;

    if (f.endsWith(".bgk")) {
        try {
            Project::open_project(f, &hex_file, &structdata);
            _emit = false;
        } catch(Project::ProjectFileException &e) {
            QMessageBox::warning(this, "Error", e.message);
        }
    } else {
        hex_file = f;
    }
    has_open_file = true;
    hexwidget->open(hex_file, _emit);
    structeditor_ui->set_struct_string(structdata);
    if (!_emit)
        add_recently_open(f);
}

void BinGrokWindow::open_recent()
{
	QAction *a = qobject_cast<QAction *>(sender());
    if (a) open_file(a->data().toString());
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
    if (!projectfile.isEmpty())
        save_file(projectfile);
    else
        save_as();
}

void BinGrokWindow::save_as()
{
    projectfile = QFileDialog::getSaveFileName(this, "Save As", last_save_dir, "BinGrok Saves (*.bgk)");
    last_save_dir = projectfile.left(projectfile.lastIndexOf("/"));
    if (projectfile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file specified");
		return;
    }
#ifndef QT_NO_DEBUG
    qDebug() << "save_as()" << projectfile;
#endif
    save_file(projectfile);
}

bool BinGrokWindow::save_file(const QString &filename)
{
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    Project::write_project(filename, hex_file, structeditor_ui->get_document());

#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

    statusBar()->showMessage(tr("Project saved"), 2000);
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

void BinGrokWindow::show_struct_editor()
{
    if (!has_open_file) {
        QMessageBox::warning(this, "Error", "Can't edit datastructure when there is no file open!");
    } else {
        structeditor_ui->show();
    }
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
    add_recently_open(f->fileName());
}

void BinGrokWindow::add_recently_open(QString f)
{
    if (recently_open.contains(f)){
		QString fname = recently_open.takeAt(
                    recently_open.indexOf(f)
					);
		recently_open.prepend(fname);
	} else {
        recently_open.prepend(f);
		while (recently_open.size() > max_recently_open) {
			recently_open.removeLast();
		}
	}
	redraw_recently_open();
}

void BinGrokWindow::redraw_recently_open()
{
    QKeySequence key;
    bool shortcut = true;

	ui->menuOpen_Recent->clear();
	for (int i = 0; i < recently_open.size(); i++) {
		QString txt = QString("&%1 %2").arg(i+1).arg(recently_open[i]);
        QAction *action = ui->menuOpen_Recent->addAction(txt, this, SLOT(open_recent()));
		action->setData(recently_open[i]);
		action->setVisible(true);
        switch(i+1) {
        case 1:
            key = Qt::Key_1 | Qt::CTRL;
            break;
        case 2:
            key = Qt::Key_2 | Qt::CTRL;
            break;
        case 3:
            key = Qt::Key_3 | Qt::CTRL;
            break;
        default:
            shortcut = false;
            break;
        }
        if (shortcut)
            action->setShortcut(key);
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
