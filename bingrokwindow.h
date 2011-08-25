#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
};

#endif // MAINWINDOW_H
