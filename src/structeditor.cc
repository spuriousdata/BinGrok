#include "structeditor.h"
#include "ui_structeditor.h"
#include "syntaxhighlighter.h"

StructEditor::StructEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StructEditor)
{
    ui->setupUi(this);
    highlighter = new SyntaxHighlighter(ui->struct_textedit->document());
}

StructEditor::~StructEditor()
{
    delete ui;
}
