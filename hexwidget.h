#ifndef BGHEXWIDGET_H
#define BGHEXWIDGET_H

#include "unistd.h"
#include <QWidget>

class QString;
class QFile;

class BGHexWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BGHexWidget(QWidget *parent = 0);
	~BGHexWidget();
	bool open(const QString & filename);

private:
	quint16 columns;
	quint16 rows;
	quint8  bytes_per_column;
	QFile   *file;

	void read_settings();
	void save();
	void close();
	bool maybe_save();
	off_t bytes_per_line();

signals:
	void update_scroll(off_t, off_t);

public slots:
	void update_preferences(const int, const QFont &);

};

#endif // BGHEXWIDGET_H
