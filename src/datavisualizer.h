#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QDialog>
#include "structtypes.h"
#include "datahighlighter.h"

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

private:
    Ui::DataVisualizer *ui;
    Struct *current_data = nullptr;
    DataHighlighter *highlighter;
};

#endif // DATAVISUALIZER_H
