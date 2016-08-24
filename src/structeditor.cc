#include "structeditor.h"
#include "ui_structeditor.h"
#include "syntaxhighlighter.h"
#include "rd_parser.h"

#include <QDialogButtonBox>
#include <QErrorMessage>
#include <QPushButton>
#include <string>

extern void parse(std::string & in);


StructEditor::StructEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructEditor)
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
    try {
        r.parse(data);
    } catch (RDParser::ParserException &e) {
        QErrorMessage m(this);
        m.showMessage(e.message);
    }
}
