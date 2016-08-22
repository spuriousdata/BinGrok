#include "txtdisplaywidget.h"

#include <QFont>
#include <QSettings>
#include <QPainter>
#include <QPalette>
#include <QApplication>
#include <QByteArray>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

TxtDisplayWidget::TxtDisplayWidget(QWidget *parent) : QWidget(parent), viewport_data(NULL)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setEnabled(true);
    setCursor(Qt::IBeamCursor);
    read_settings();
}

void TxtDisplayWidget::read_settings()
{
    QSettings s;
    s.beginGroup("hexview");
    //bytes_per_column = s.value("bytes_per_column", 1).toInt();

    QFont f("Courier");
    f.setStyleHint(QFont::TypeWriter);
    f.setPointSize(s.value("fontsize", 12).toInt());
    setFont(f);

    s.endGroup();
}

void TxtDisplayWidget::trigger_resizeEvent()
{
    QResizeEvent *e = new QResizeEvent(size(), size());
    resizeEvent(e);
    delete e;
}

void TxtDisplayWidget::set_viewport_data(QByteArray *v)
{
    viewport_data = v;
}

void TxtDisplayWidget::set_trtable(TrTable *t)
{
    trtable = t;
}

void TxtDisplayWidget::set_selection(Selection *s)
{
    sel = s;
}

void TxtDisplayWidget::set_seek_to(quint64 *st)
{
    seek_to = st;
}

void TxtDisplayWidget::update_grid_sizes()
{
    col_width  = fontMetrics().width("0"); // character width
    row_height = fontMetrics().height();   // character height

    columns = size().width() / col_width;
    rows = size().height() / row_height;
}

QString TxtDisplayWidget::get_dataword(quint32 offset)
{
    QString data;

    if (viewport_data == NULL) data.append(" ");
    else {
        if ((quint32)viewport_data->size() <= offset) data.append(" ");
        else data.append(trtable->get_char(viewport_data->at(offset)));
    }

    return data;
}

/******************************************************************************
 *
 *                             Overrides
 *
 *****************************************************************************/

void TxtDisplayWidget::resizeEvent(QResizeEvent *e)
{
#ifndef QT_NO_DEBUG
    qDebug() << "resizeEvent(" << e->size().width() << ", " <<
                e->size().height() << ")";
#endif

    update_grid_sizes();
    /*
    if (file != NULL) {
#ifndef QT_NO_DEBUG
        qDebug() << "file->size() " << file->size();
        qDebug() << "bytes_per_line() * rows " << bytes_per_line() * rows;
#endif
        bytes_per_page = bytes_per_line() * rows;
        scroll_lines = ((file->size() - bytes_per_page) / bytes_per_line()) + 1;
        scrollbar->setRange(0, scroll_lines);
    }
    */
}

void TxtDisplayWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.setFont(font());
    QPalette palette = QApplication::palette(this);
    painter.setPen(palette.foreground().color());
    painter.setBackgroundMode(Qt::OpaqueMode);

    quint32 i = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            QString word = get_dataword(i++);

            if (sel != NULL) {
                if (sel->in_range(c, r, *seek_to)) {
                    painter.setBackground(palette.link());
                    painter.setPen(palette.brightText().color());
                } else {
                    painter.setBackground(palette.base());
                    painter.setPen(palette.foreground().color());
                }
            }

            painter.drawText(c*col_width, r*row_height, col_width, row_height, Qt::AlignLeft, word);
        }
    }
}
