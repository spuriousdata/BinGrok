#include "hexwidget.h"
#include "trtable.h"

#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QIODevice>
#include <QPainter>
#include <QPalette>
#include <QApplication>
#include <QFont>
#include <QResizeEvent>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

HexWidget::HexWidget(QWidget *parent) :
	QWidget(parent),file(NULL)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	columns = 20;
	rows = 20;

	read_settings();
}

void HexWidget::read_settings()
{
	QSettings s;
	s.beginGroup("hexview");
	bytes_per_column = s.value("bytes_per_column", 1).toInt();

	setFont(s.value("font", QFont("Courier")).value<QFont>());

	s.endGroup();
}

void HexWidget::write_settings()
{
	QSettings s;
	s.beginGroup("hexview");
	s.setValue("bytes_per_column", bytes_per_column);
	s.setValue("font", font());
	s.endGroup();
}

bool HexWidget::open(const QString & filename)
{
	if (!maybe_save())
		return false;

	close();

	file = new QFile(filename);
	if (!file->open(QIODevice::ReadWrite)) {
		QMessageBox::critical(this, "Error", "Error Opening file '" +
									filename  + "'" + file->errorString(),
							  QMessageBox::Ok, 0);
		close();
		return false;
	} /* else success */

	emit file_opened(file);

	trigger_resizeEvent();
	update();
	return true;
}

void HexWidget::trigger_resizeEvent()
{
	QResizeEvent *e = new QResizeEvent(size(), size());
	resizeEvent(e);
	delete e;
}

off_t HexWidget::bytes_per_line()
{
	return bytes_per_column * columns;
}

bool HexWidget::maybe_save()
{
	/* Is there something to save? */
	if (file == NULL)
		return true;

	/*
	if (file not modified)
		return true;
	*/

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

void HexWidget::save()
{

}

void HexWidget::close()
{
	if (file != NULL) {
		file->close();
		delete file;
		file = NULL;
	}
	// clear widget display
}

void HexWidget::update_grid_sizes()
{
	int cwidth  = fontMetrics().width("0"); // character width
	int cheight = fontMetrics().height();   // character height
	col_width = (cwidth * 2 * bytes_per_column) + 10; // 10px space
	row_height = cheight + 10; // 10px space

	columns = size().width() / col_width;
	rows = size().height() / row_height;
}

void HexWidget::update_preferences(int bpc, const QFont & f)
{
	bytes_per_column = bpc;
	setFont(f);

	write_settings();
	trigger_resizeEvent();
}

void HexWidget::update_viewport_data()
{
	if (file == NULL) {
		viewport_data.clear();
		return;
	}

	file->seek(0); // this is obviously wrong.
	viewport_data = file->read(rows*columns);
}


QString HexWidget::get_dataword(quint32 offset)
{
	QString data;

	for (int i = 0; i < bytes_per_column; i++) {
		if (file == NULL) data.append(" ");
		else {
			int x = offset+i;
			if (viewport_data.size() < x) data.append(" ");
			else data.append(trtable.get_hex(viewport_data[x]));
		}
	}

	return data;
}

/******************************************************************************
 *
 *                             Overrides
 *
 *****************************************************************************/

void HexWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	painter.setFont(font());
	QPalette palette = QApplication::palette(this);
	painter.setPen(palette.foreground().color());

	quint32 i = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			QString word = get_dataword(i);
			i += bytes_per_column;
			painter.drawText(c*col_width, r*row_height,
							 col_width, row_height, Qt::AlignLeft,
							 word
							 );
		}
	}
}

void HexWidget::resizeEvent(QResizeEvent *e)
{
#ifndef QT_NO_DEBUG
	qDebug() << "resizeEvent(" << e->size().width() << ", " <<
				e->size().height() << ")";
#endif

	update_grid_sizes();
	update_viewport_data();
	if (file != NULL) {
#ifndef QT_NO_DEBUG
		qDebug() << "file->size() " << file->size();
		qDebug() << "bytes_per_line() * rows " << bytes_per_line() * rows;
#endif
		emit update_scroll(0, file->size() / (bytes_per_line() * rows));
	}
}

HexWidget::~HexWidget()
{
	close();
}
