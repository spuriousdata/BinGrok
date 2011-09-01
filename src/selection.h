#ifndef SELECTION_H
#define SELECTION_H

#include <QPoint>

class Selection
{
public:
	Selection(int columns) : columns(columns) {};

	void start(quint64 x, quint64 y, quint64 tell);
	void start(QPoint p, quint64 tell) { start(p.x(), p.y(), tell); }

	void end(quint64 x, quint64 y, quint64 tell);
	void end(QPoint p, quint64 tell) { end(p.x(), p.y(), tell); }

	bool in_range(quint64 x, quint64 y, quint64 tell);

private:
	quint64 columns;
	quint64 start_offset, end_offset;

	quint64 xy_to_offset(quint64 x, quint64 y, quint64 tell);
};

#endif // SELECTION_H
