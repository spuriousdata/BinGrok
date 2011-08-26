#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QString;
class BGHexWidget;
class QScrollBar;
class QFile;

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
	BGHexWidget *hexwidget;
	QScrollBar *vscroll;

	void read_settings();
	bool save_file(const QString & filename);
	void closeEvent(QCloseEvent *e);

private slots:
	void new_file();
	void open();
	void save();
	void save_as();
	void update_scroll(off_t, off_t);
};

#endif // MAINWINDOW_H
