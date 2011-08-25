#include <QtGui/QApplication>
#include <QCoreApplication>
#include "bingrokwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	BinGrokWindow w;

	QCoreApplication::setOrganizationDomain("spuriousdata.com");
	QCoreApplication::setOrganizationName("spuriousdata");
	QCoreApplication::setApplicationName("BinGrok");

    w.show();

    return a.exec();
}
