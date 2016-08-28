#include "hexwidget.h"
#include "trtable.h"
#include "txtdisplaywidget.h"

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
#include <QTimer>
#include <QScrollBar>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

HexWidget::HexWidget(QWidget *parent) :
	QWidget(parent),file(NULL),seek_to(0),
    columns(20),rows(20),sel(new Selection(0, 0)), grokked(new Selection(0, 0)),
    txtdisplay(NULL), mouse_down(false)
{
	scroll_timer = new QTimer(this);
    connect(scroll_timer, &QTimer::timeout, this, &HexWidget::drag_scroll);
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	setEnabled(true);
	setCursor(Qt::IBeamCursor);
	read_settings();
}

void HexWidget::set_txtdisplay(TxtDisplayWidget *t)
{
    txtdisplay = t;
    txtdisplay->set_viewport_data(&viewport_data);
    txtdisplay->set_trtable(&trtable);
    txtdisplay->set_seek_to(&seek_to);
    connect(this, SIGNAL(updating()), txtdisplay, SLOT(update()));
}

void HexWidget::read_settings()
{
	QSettings s;
	s.beginGroup("hexview");
	bytes_per_column = s.value("bytes_per_column", 1).toInt();

	QFont f("Courier");
	f.setStyleHint(QFont::TypeWriter);
	f.setPointSize(s.value("fontsize", 12).toInt());
	setFont(f);

	s.endGroup();
}

void HexWidget::write_settings()
{
	QSettings s;
	s.beginGroup("hexview");
	s.setValue("bytes_per_column", bytes_per_column);
	s.setValue("fontsize", font().pointSize());
	s.endGroup();
}

bool HexWidget::open(const QString & filename, bool _emit)
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

    if (_emit)
        emit file_opened(file);

	trigger_resizeEvent();

    sel->resize(columns, file->size());
    grokked->resize(columns, file->size());
    txtdisplay->set_selection(sel);
    txtdisplay->set_grokked(grokked);

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
    txtdisplay->trigger_resizeEvent();
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


	QString warning = "Save before closing \"" + file->fileName() + "\"?";
	int s = QMessageBox::warning(this, "File Modified", warning,
								 QMessageBox::Yes, QMessageBox::No,
								 QMessageBox::Cancel
							 );

	if (s == QMessageBox::Cancel)
		return false;

	if (s == QMessageBox::Yes)
		save();
    */

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
	col_width = (cwidth * 2 * bytes_per_column) + cwidth; //add cwdith again for final space
	row_height = cheight;

	columns = size().width() / col_width;
	rows = size().height() / row_height;
}

void HexWidget::update_preferences(int bpc, int fsz)
{
	bytes_per_column = bpc;
	QFont f = font();
	f.setPointSize(fsz);
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
	viewport_data = file->read(rows*columns*bytes_per_column);
}

const char *HexWidget::read_bytes(qint64 from, qint64 length) const
{
    qint64 tell = file->pos();
    char *data = new char[length];
    file->seek(from);
    file->read(data, length);

    // restore state
    file->seek(tell);

    return data;
}


QString HexWidget::get_dataword(quint32 offset)
{
	QString data;

	for (int i = 0; i < bytes_per_column; i++) {
		if (file == NULL) data.append(" ");
		else {
            uint x = offset+i;
            if (static_cast<uint>(viewport_data.size()) <= x) data.append(" ");
			else data.append(trtable.get_hex(viewport_data[x]));
		}
	}

	data.append(" "); // for nice column lines

	return data;
}

void HexWidget::set_scrollbar(QScrollBar *s)
{
	scrollbar = s;
	connect(scrollbar, SIGNAL(valueChanged(int)),
			this, SLOT(scroll_changed(int)));
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
        if (sel->is_active())
            sel->reset();

		sel->start(xy_to_grid(e), seek_to);
	} else {
		sel->end(xy_to_grid(e), seek_to);
	}
    update();
}

QPoint HexWidget::xy_to_grid(QMouseEvent *e)
{
	QPoint pos = e->pos();

	int ecol = pos.x() / col_width;
	int erow = pos.y() / row_height;
	return QPoint(ecol, erow);
}

void HexWidget::drag_scroll()
{
	if (scroll_direction == DOWN) {
		scrollbar->setSliderPosition(scrollbar->sliderPosition()+scroll_speed);
	} else {
		scrollbar->setSliderPosition(scrollbar->sliderPosition()-scroll_speed);
	}
}

void HexWidget::highlight(quint64 start_tell, quint64 end_tell)
{
    grokked->select(start_tell, end_tell);
    update();
}

void HexWidget::reset_highlight()
{
    grokked->reset();
    update();
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
	painter.setBackgroundMode(Qt::OpaqueMode);

	quint32 i = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			QString word = get_dataword(i);
			i += bytes_per_column;

            // put selection first so that you can "see" it over the grokked bytes
            if (sel->is_active() && sel->in_range(c, r, seek_to)) {
                painter.setBackground(palette.link());
                painter.setPen(palette.brightText().color());
            } else if (grokked->is_active() && grokked->in_range(c, r, seek_to)) {
                painter.setBackground(palette.highlight());
                painter.setPen(palette.brightText().color());
            } else {
                painter.setBackground(palette.base());
                painter.setPen(palette.foreground().color());
            }
			painter.drawText(c*col_width, r*row_height,
							 col_width, row_height, Qt::AlignLeft,
							 word);
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
		scrollbar->setRange(0, scroll_lines);
        sel->resize(columns);
        grokked->resize(columns);
	}
}

void HexWidget::wheelEvent(QWheelEvent *e)
{
	if (e->delta() > 0) {
		scrollbar->setSliderPosition(scrollbar->sliderPosition()-3);
	} else {
		scrollbar->setSliderPosition(scrollbar->sliderPosition()+3);
	}
	e->accept();
}

void HexWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (sel->is_active()) {
            sel->reset();
            update();
        }
        mouse_down = true;
        e->accept();
    }
}

void HexWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		mouse_down = false;
        if (sel->is_active()) {
            selection(e, false);
			scroll_timer->stop();
            update();
		}
		e->accept();
	}
}

void HexWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!sel->is_active() && mouse_down) {
		selection(e, true);
    } else if (sel->is_active() && mouse_down) {
        selection(e, false);
		e->accept();

		/* if moved below or above widget bounds
		 * start timer to emit scroll wheel events up or down
		 * -- decrease interval for increasing distance from widget bounds
		 */
		if (e->pos().y() > size().height()) {
			// scroll down
			scroll_direction = DOWN;
			scroll_speed = (e->pos().y() - size().height()) / 25;
			if (!scroll_timer->isActive())
				scroll_timer->start(30);
		} else if (e->pos().y() < 0) {
			scroll_direction = UP;
			scroll_speed = (e->pos().y()*-1) / 25;
			if (!scroll_timer->isActive())
				scroll_timer->start(30);
		} else {
			scroll_timer->stop();
		}
	}
}

void HexWidget::update()
{
    QWidget::update();
    emit updating();
}

HexWidget::~HexWidget()
{
	close();
}
