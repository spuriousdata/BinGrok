#ifndef DATAVISWIDGET_H
#define DATAVISWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>

class DataVisWidget : public QTextEdit
{
    Q_OBJECT

public:
    DataVisWidget(QWidget *parent=0);

protected:
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // DATAVISWIDGET_H
