#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QDialog>
#include <QLayoutItem>
#include "structtypes.h"
#include "datahighlighter.h"
#include "dataviswidget.h"

namespace Ui {
class DataVisualizer;
}

class DataVisualizer : public QDialog
{
    Q_OBJECT

public:
    explicit DataVisualizer(QWidget *parent = 0);
    ~DataVisualizer();
    void visualize(Struct *s);

protected:
    void mouseReleaseEvent(QMouseEvent *);

private:
    Ui::DataVisualizer *ui;

    Struct *current_data = nullptr;
    DataHighlighter *highlighter;
    DataVisWidget *text_display;
    QLayoutItem *text_display_li;
};

#endif // DATAVISUALIZER_H
