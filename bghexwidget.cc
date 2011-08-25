#include "bghexwidget.h"
#include <QSettings>

BGHexWidget::BGHexWidget(QWidget *parent) :
    QWidget(parent)
{
	read_settings();
}

void BGHexWidget::read_settings()
{
	QSettings settings;
	settings.beginGroup("bghexwidget");

	columns = settings.value("columns", 20).toInt();
	rows = settings.value("rows", 20).toInt();
	bytes_per_column = settings.value("bytes_per_column", 1).toInt();

	settings.endGroup();
}

BGHexWidget::~BGHexWidget()
{
}
