#ifndef SELECTION_H
#define SELECTION_H

#include <QPoint>

class Selection
{
public:
	Selection() {};
	void start(int col, int row) { _start = QPoint(col, row); }
	void start(QPoint p) { _start = p; }
	void end(int col, int row) { _end = QPoint(col, row); }
	void end(QPoint p) { _end = p; }
	bool in_range(int col, int row, int columns);

private:
	QPoint _start;
	QPoint _end;
};

#endif // SELECTION_H
