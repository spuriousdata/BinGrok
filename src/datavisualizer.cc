#include "datavisualizer.h"
#include "ui_datavisualizer.h"

DataVisualizer::DataVisualizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataVisualizer)
{
    ui->setupUi(this);
    ui->status_panel->setPlainText("test");
    highlighter = new DataHighlighter(ui->text_display->document());
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
    ui->text_display->setText(s->to_string());
}
