#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QString;
class HexWidget;
class QScrollBar;
class QFile;
class QWidget;
class QHBoxLayout;

class Preferences;

namespace Ui {
	class BinGrokWindow;
}

class BinGrokWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit BinGrokWindow(QWidget *parent = 0);
	~BinGrokWindow();

private:
	Ui::BinGrokWindow *ui;
	Preferences *preferences_ui;
	HexWidget *hexwidget;
	QScrollBar *vscroll;
	QWidget *container;
	QHBoxLayout *layout;
	quint8 max_recently_open;
	QStringList recently_open;

	void read_settings();
	bool save_file(const QString & filename);
	void closeEvent(QCloseEvent *e);
	void redraw_recently_open();

public slots:
	void new_file();
	void open();
	void open_recent();
	void save();
	void save_as();
	void show_preferences();
	void save_preferences();
	void update_scroll(off_t, off_t);
	void add_recently_open(QFile *);
};

#endif // MAINWINDOW_H
