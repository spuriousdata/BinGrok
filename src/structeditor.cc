#include "structeditor.h"
#include "ui_structeditor.h"
#include "syntaxhighlighter.h"
#include "rd_parser.h"
#include "structtypes.h"

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

    QPushButton *apply_button = ui->buttonBox->button(QDialogButtonBox::Apply);

    connect(apply_button, SIGNAL(clicked()), this, SLOT(apply_struct()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(close_and_apply()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

StructEditor::~StructEditor()
{
    delete ui;
    delete error_window;
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
    Struct *s;
    try {
        s = r.parse(data);
    } catch (RDParser::ParserException &e) {
        error_window->setWindowTitle("Parse Error");
        error_window->setWindowFlags(Qt::WindowStaysOnTopHint);
        error_window->setText(e.message);
        error_window->show();
    }
}
