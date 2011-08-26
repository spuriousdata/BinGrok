#include "bghexwidget.h"
#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QIODevice>

BGHexWidget::BGHexWidget(QWidget *parent) :
	QWidget(parent),file(NULL)
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

bool BGHexWidget::open(const QString & filename)
{
	if (!maybe_save())
		return false;

	if (file != NULL)
		close();

	file = new QFile(filename);
	if (!file->open(QIODevice::ReadWrite)) {
		QMessageBox::critical(this, "Error", "Error Opening file '" + filename  + "'" + file->errorString(), QMessageBox::Ok, 0);
		close();
		return false;
	}

	// clear modification status
	//
	emit update_scroll(0,file->size()/bytes_per_line());

	return true;
}

off_t BGHexWidget::bytes_per_line()
{
	return bytes_per_column * columns;
}

bool BGHexWidget::maybe_save()
{
	/* Is there something to save? */
	if (file == NULL)
		return true;

	QString warning = "Save before closing \"" + file->fileName() + "\"?";
	int s = QMessageBox::warning(this, "File Modified", warning,
							 QMessageBox::Yes, QMessageBox::No,
							 QMessageBox::Cancel
							 );

	if (s == QMessageBox::Cancel)
		return false;

	if (s == QMessageBox::Yes)
		save();

	return true;
}

void BGHexWidget::save()
{

}

void BGHexWidget::close()
{
	file->close();
	delete file;
	file = NULL;
	// clear widget display
}


BGHexWidget::~BGHexWidget()
{
}
