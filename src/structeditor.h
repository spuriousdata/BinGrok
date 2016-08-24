#ifndef STRUCTEDITOR_H
#define STRUCTEDITOR_H

#include <QDialog>
#include <QList>
#include "syntaxhighlighter.h"
#include "structtypes.h"

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
    QList<StructStatement*> structs;

public slots:
    void apply_struct();
    void close_and_apply();
    //void close();
};

#endif // STRUCTEDITOR_H
