#ifndef BGHEXWIDGET_H
#define BGHEXWIDGET_H

#include "trtable.h"
#include "selection.h"

#include "unistd.h"
#include <QWidget>
#include <QByteArray>
#include <QFont>
#include <QString>

class QFile;
class QPaintEvent;
class QMouseEvent;
class QScrollBar;

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

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

private:
	quint16     columns;
	quint16     rows;
	quint8      bytes_per_column;
	quint64     seek_to;
	quint32     bytes_per_page;
	quint32     scroll_lines;
	QFile      *file;
	off_t       cur_offset;
	int         col_width;
	int         row_height;
	QByteArray  viewport_data;
	TrTable     trtable;
	Selection  *sel;
	QScrollBar *scrollbar;

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

public slots:
	void update_preferences(const int, const QFont &);
	void scroll_changed(int i);

};

#endif // BGHEXWIDGET_H
