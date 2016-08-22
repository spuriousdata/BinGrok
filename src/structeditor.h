#ifndef STRUCTEDITOR_H
#define STRUCTEDITOR_H

#include <QDialog>
#include "syntaxhighlighter.h"

namespace Ui {
    class StructEditor;
}

class StructEditor : public QDialog
{
    Q_OBJECT

public:
    explicit StructEditor(QWidget *parent = 0);
    ~StructEditor();

private:
    Ui::StructEditor *ui;
    SyntaxHighlighter *highlighter;
};

#endif // STRUCTEDITOR_H
