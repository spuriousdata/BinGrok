#include "structeditor.h"
#include "ui_structeditor.h"
#include "syntaxhighlighter.h"
#include "bingrokwindow.h"
#include "rd_parser.h"
#include "structtypes.h"
#include "datavisualizer.h"
#include "hexwidget.h"

#include <QDialogButtonBox>
#include <QErrorMessage>
#include <QException>
#include <QPushButton>
#include <string>

extern void parse(std::string & in);


StructEditor::StructEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructEditor),
    error_window(new QMessageBox)
{
    ui->setupUi(this);
    highlighter = new SyntaxHighlighter(ui->struct_textedit->document());
    hex_display = static_cast<BinGrokWindow*>(parent)->get_hexwidget();

    QPushButton *apply_button = ui->buttonBox->button(QDialogButtonBox::Apply);

    connect(apply_button, SIGNAL(clicked()), this, SLOT(apply_struct()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(close_and_apply()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(this, &StructEditor::struct_applied, hex_display, &HexWidget::highlight);
}

StructEditor::~StructEditor()
{
    delete ui;
    delete error_window;
}

void StructEditor::show_datavisualizer(Struct *s)
{
    if (dv == nullptr)
        dv = new DataVisualizer(this);

    dv->visualize(s);
    dv->show();
}

void StructEditor::close_and_apply()
{
    apply_struct();
    close();
}

void StructEditor::apply_struct()
{
    QString data = ui->struct_textedit->document()->toPlainText();
    RDParser r;
    Struct *s = nullptr;
    quint64 start_offset = ui->start_at->text().toLong();
    const char *d = nullptr;

    try {
        s = r.parse(data);
        d = hex_display->read_bytes(start_offset, s->record_length());
        s->populate(d);
        show_datavisualizer(s);
        emit struct_applied(start_offset, s->record_length()-1);
    } catch (RDParser::ParserException &e) {
        error_window->setWindowTitle("Parse Error");
        error_window->setWindowFlags(Qt::WindowStaysOnTopHint);
        error_window->setText(e.message);
        error_window->show();
    }
}
