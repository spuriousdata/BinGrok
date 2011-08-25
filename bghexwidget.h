#ifndef BGHEXWIDGET_H
#define BGHEXWIDGET_H

#include <QWidget>

class BGHexWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BGHexWidget(QWidget *parent = 0);
	~BGHexWidget();

private:
	qint16 columns;
	qint8  rows;
	qint8  bytes_per_column;

	void read_settings();

signals:

public slots:

};

#endif // BGHEXWIDGET_H
