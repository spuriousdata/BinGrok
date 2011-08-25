#include "bingrokwindow.h"
#include "ui_bingrokwindow.h"

BinGrokWindow::BinGrokWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::BinGrokWindow)
{
    ui->setupUi(this);
}

BinGrokWindow::~BinGrokWindow()
{
    delete ui;
}
