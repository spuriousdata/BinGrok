#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QString;

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
	void read_settings();
	bool save_file(const QString & filename);
	void closeEvent(QCloseEvent *e);

private slots:
	void new_file();
	void open();
	bool save();
	bool save_as();
};

#endif // MAINWINDOW_H
