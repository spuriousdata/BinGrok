#ifndef BGHEXWIDGET_H
#define BGHEXWIDGET_H

#include "trtable.h"

#include "unistd.h"
#include <QWidget>
#include <QByteArray>
#include <QFont>
#include <QString>

class QFile;
class QPaintEvent;

class HexWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HexWidget(QWidget *parent = 0);
	~HexWidget();
	bool open(const QString & filename);
	bool maybe_save();
	inline quint8 get_bytes_per_column() { return bytes_per_column; }

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

private:
	quint16    columns;
	quint16    rows;
	quint8     bytes_per_column;
	QFile     *file;
	off_t      cur_offset;
	int        col_width;
	int	       row_height;
	QByteArray viewport_data;
	TrTable    trtable;

	off_t   bytes_per_line();
	void    read_settings();
	void    write_settings();
	void    save();
	void    close();
	void    update_grid_sizes();
	void    update_viewport_data();
	void    trigger_resizeEvent();
	QString get_dataword(quint32 offset);


signals:
	void update_scroll(off_t, off_t);

public slots:
	void update_preferences(const int, const QFont &);

};

#endif // BGHEXWIDGET_H
