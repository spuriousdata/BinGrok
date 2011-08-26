#include <QtGui/QApplication>
#include <QCoreApplication>
#include "bingrokwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	/* These MUST preceed BinGrokWindow constructor */
	QCoreApplication::setOrganizationDomain("spuriousdata.com");
	QCoreApplication::setOrganizationName("spuriousdata");
	QCoreApplication::setApplicationName("BinGrok");

	BinGrokWindow w;

    w.show();

    return a.exec();
}
