#ifndef TXTDISPLAYWIDGET_H
#define TXTDISPLAYWIDGET_H

#include <QWidget>
#include <QByteArray>
#include <QPaintEvent>
#include "trtable.h"
#include "selection.h"

class TxtDisplayWidget : public QWidget
{
    Q_OBJECT
    friend class HexWidget;
public:
    explicit TxtDisplayWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);

private:
    QByteArray  *viewport_data;
    quint16     columns;
    quint16     rows;
    int         col_width;
    int         row_height;
    TrTable     *trtable;
    Selection   *sel;
    quint64     *seek_to;

    void    trigger_resizeEvent();
    void    update_grid_sizes();
    QString get_dataword(quint32 offset);
    void resizeEvent(QResizeEvent *e);
    void set_viewport_data(QByteArray *v);
    void set_trtable(TrTable *t);
    void set_selection(Selection *s);
    void set_seek_to(quint64 *st);
    void read_settings();

signals:

public slots:
};

#endif // TXTDISPLAYWIDGET_H
