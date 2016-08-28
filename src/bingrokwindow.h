#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QMessageBox>
#include "hexwidget.h"

class QString;
class TxtDisplayWidget;
class QScrollBar;
class QFile;
class QWidget;
class QHBoxLayout;

class Preferences;
class StructEditor;

namespace Ui {
	class BinGrokWindow;
}

class BinGrokWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit BinGrokWindow(QWidget *parent = 0);
	~BinGrokWindow();
    HexWidget *get_hexwidget() { return hexwidget; }

private:
	Ui::BinGrokWindow *ui;
	Preferences *preferences_ui;
    StructEditor *structeditor_ui;
	HexWidget *hexwidget;
    TxtDisplayWidget *txtdisplaywidget;
	QScrollBar *vscroll;
	QWidget *container;
	QHBoxLayout *layout;
	quint8 max_recently_open;
	QStringList recently_open;
    bool has_open_file = false;
    QString hex_file;
    QString projectfile;
    QString last_save_dir;

	void read_settings();
	bool save_file(const QString & filename);
	void closeEvent(QCloseEvent *e);
	void redraw_recently_open();

public slots:
	void new_file();
	void open();
	void open_recent();
	void open_file(const QString &);
	void save();
	void save_as();
	void show_preferences();
    void show_struct_editor();
	void save_preferences();
	void add_recently_open(QFile *);
    void add_recently_open(QString f);
};

#endif // MAINWINDOW_H
