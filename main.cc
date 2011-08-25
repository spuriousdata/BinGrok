#include <QtGui/QApplication>
#include "bingrokwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	BinGrokWindow w;
    w.show();

    return a.exec();
}
