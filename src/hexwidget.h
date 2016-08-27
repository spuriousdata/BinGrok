#ifndef BGHEXWIDGET_H
#define BGHEXWIDGET_H

#include "trtable.h"
#include "selection.h"

#include "unistd.h"
#include <QWidget>
#include <QByteArray>
#include <QFont>
#include <QString>
#include <QPoint>

class QFile;
class QPaintEvent;
class QMouseEvent;
class QTimer;
class QScrollBar;
class TxtDisplayWidget;

class HexWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HexWidget(QWidget *parent = 0);
	~HexWidget();
	void new_file();
	bool open(const QString & filename);
	bool maybe_save();
	inline quint8 get_bytes_per_column() { return bytes_per_column; }
	void set_scrollbar(QScrollBar *s);
    void set_txtdisplay(TxtDisplayWidget *t);
    bool get_display_textview() { return display_textview; }
    const char *read_bytes(qint64 from, qint64 length) const;

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

private:
    QFile      *file;
    qint64      seek_to;
    int         columns;
    int         rows;
	quint8      bytes_per_column;
	quint32     bytes_per_page;
	quint32     scroll_lines;
	off_t       cur_offset;
	int         col_width;
	int         row_height;
	QByteArray  viewport_data;
	TrTable     trtable;
	Selection  *sel;
	QPoint      mouse_position;
	QTimer     *scroll_timer;
	QScrollBar *scrollbar;
	enum SCROLL_DIRECTION { UP=0, DOWN=2 } scroll_direction;
	quint8      scroll_speed;
    TxtDisplayWidget *txtdisplay;
    bool display_textview;
    bool        mouse_down;

	off_t   bytes_per_line();
	void    read_settings();
	void    write_settings();
	void    save();
	void    close();
	void    update_grid_sizes();
	void    update_viewport_data();
	void    trigger_resizeEvent();
	QString get_dataword(quint32 offset);
	void    selection(QMouseEvent *e, bool new_selection);
	QPoint  xy_to_grid(QMouseEvent *e);


signals:
	void file_opened(QFile *);
    void updating();

public slots:
	void update_preferences(const int, int);
	void scroll_changed(int i);
	void drag_scroll();
    void update();

};

#endif // BGHEXWIDGET_H
