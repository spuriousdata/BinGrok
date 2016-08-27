#ifndef STRUCTEDITOR_H
#define STRUCTEDITOR_H

#include <QDialog>
#include <QList>
#include <QMessageBox>
#include "hexwidget.h"
#include "syntaxhighlighter.h"
#include "datavisualizer.h"
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
    DataVisualizer *dv = nullptr;
    QList<StructStatement*> structs;
    QMessageBox *error_window;
    HexWidget *hex_display;

    void show_datavisualizer(Struct *s);

public slots:
    void apply_struct();
    void close_and_apply();
    //void close();

signals:
    void struct_applied(quint64 start, quint64 end);
};

#endif // STRUCTEDITOR_H
