#include "dataviswidget.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

DataVisWidget::DataVisWidget(QWidget *parent) : QTextEdit(parent)
{
    this->setReadOnly(true);
}

void DataVisWidget::mouseReleaseEvent(QMouseEvent *e)
{
#ifndef QT_NO_DEBUG
    qDebug() << "Mouse Event: " << e;
#endif
}
