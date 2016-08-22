#include <QApplication>
#include <QCoreApplication>
#include <QStringList>
#include "bingrokwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	/* These MUST preceed BinGrokWindow constructor */
	QCoreApplication::setOrganizationDomain("spuriousdata.com");
	QCoreApplication::setOrganizationName("spuriousdata");
	QCoreApplication::setApplicationName("BinGrok");

	BinGrokWindow w;

	QStringList args = qApp->arguments();
	if (args.size() > 1)
		w.open_file(args.at(1));

    w.show();

    return a.exec();
}
