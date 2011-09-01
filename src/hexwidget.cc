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
#include <QCursor>
#include <QMouseEvent>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

HexWidget::HexWidget(QWidget *parent) :
	QWidget(parent),file(NULL),seek_to(0),
	columns(20),rows(20),sel(NULL)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	setEnabled(true);
	setCursor(Qt::IBeamCursor);
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

void HexWidget::new_file()
{
	if (!maybe_save())
		return;
	close();
	trigger_resizeEvent();
	update();
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

	file->seek(seek_to);
	viewport_data = file->read(rows*columns);
}


QString HexWidget::get_dataword(quint32 offset)
{
	QString data;

	for (int i = 0; i < bytes_per_column; i++) {
		if (file == NULL) data.append(" ");
		else {
			int x = offset+i;
			if (viewport_data.size() <= x) data.append(" ");
			else data.append(trtable.get_hex(viewport_data[x]));
		}
	}

	return data;
}

void HexWidget::scroll_changed(int i)
{
#ifndef QT_NO_DEBUG
	qDebug() << "Scroll changed to " << i;
#endif
	seek_to = i * bytes_per_line();
	update_viewport_data();
	update();
}

void HexWidget::selection(QMouseEvent *e, bool new_selection=false)
{
	if (viewport_data.isEmpty())
		return;

	if (new_selection) {
		if (sel != NULL) {
			delete sel;
		}
		sel = new Selection(columns);
		sel->start(xy_to_grid(e), seek_to);
	} else {
		sel->end(xy_to_grid(e), seek_to);
	}
}

QPoint HexWidget::xy_to_grid(QMouseEvent *e)
{
	QPoint pos = e->pos();

	int ecol = pos.x() / col_width;
	int erow = pos.y() / row_height;
	return QPoint(ecol, erow);
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
	painter.setBackgroundMode(Qt::OpaqueMode);

	quint32 i = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			QString word = get_dataword(i);
			i += bytes_per_column;

			if (sel != NULL) {
				if (sel->in_range(c, r, seek_to)) {
					painter.setBackground(palette.link());
					painter.setPen(palette.brightText().color());
				} else {
					painter.setBackground(palette.base());
					painter.setPen(palette.foreground().color());
				}
			}
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
		bytes_per_page = bytes_per_line() * rows;
		scroll_lines = ((file->size() - bytes_per_page) / bytes_per_line()) + 1;
		emit update_scroll(0, scroll_lines);
	}
}

void HexWidget::wheelEvent(QWheelEvent *e)
{
	if (e->delta() > 0) {
		// positive == scroll up
		emit scroll_wheel_changed(-1);
	} else {
		// negative == scroll down
		emit scroll_wheel_changed(1);
	}
	e->accept();
}

void HexWidget::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		selection(e, true);
		e->accept();
	}
}

void HexWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
#ifndef QT_NO_DEBUG
		qDebug() << "left mouse released";
#endif
		selection(e);
		update();
		e->accept();
	}
}

void HexWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (sel != NULL) {
		sel->end(xy_to_grid(e), seek_to);
		update();
		e->accept();

		/* if moved below or above widget bounds
		 * start timer to emit scroll wheel events up or down
		 * -- decrease interval for increasing distance from widget bounds
		 */
	}
}

HexWidget::~HexWidget()
{
	close();
}
