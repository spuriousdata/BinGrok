#include "datavisualizer.h"
#include "ui_datavisualizer.h"
#include <QTextEdit>
#include <QWidgetItem>
#include <QSizePolicy>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

DataVisualizer::DataVisualizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataVisualizer)
{
    ui->setupUi(this);
    text_display = new DataVisWidget(this);
    highlighter = new DataHighlighter(text_display->document());
    text_display_li = new QWidgetItem(text_display);
    QSizePolicy p1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    p1.setHorizontalStretch(1); // stretch factor 3 (one byte takes up 3 columns including it's space)
    text_display->setSizePolicy(p1);

    ui->horizontalLayout->insertItem(0, text_display_li);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(close()));
}

DataVisualizer::~DataVisualizer()
{
    delete ui;
}

void DataVisualizer::visualize(Struct *s)
{
    if (current_data != nullptr)
        delete current_data;
    current_data = s;
    text_display->setText(s->to_string());
}

void DataVisualizer::mouseReleaseEvent(QMouseEvent *e)
{
#ifndef QT_NO_DEBUG
    qDebug() << "Mouse Release Event" << e;
#endif
}
