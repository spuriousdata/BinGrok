#ifndef SELECTION_H
#define SELECTION_H

#include <QPoint>

class Selection
{
public:
	Selection(int columns, quint64 size) : columns(columns),size(size) {};

	void start(int x, int y, quint64 tell);
	void start(QPoint p, quint64 tell) { start(p.x(), p.y(), tell); }

	void end(int x, int y, quint64 tell);
	void end(QPoint p, quint64 tell) { end(p.x(), p.y(), tell); }

	bool in_range(int x, int y, quint64 tell);

private:
	int columns, size;
	quint64 start_offset, end_offset;

	quint64 xy_to_offset(int x, int y, quint64 tell);
};

#endif // SELECTION_H
