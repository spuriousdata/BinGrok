#include "selection.h"
#include <QPoint>

void Selection::start(quint64 x, quint64 y, quint64 tell)
{
	start_offset = xy_to_offset(x, y, tell);
	end_offset = start_offset;
}

void Selection::end(quint64 x, quint64 y, quint64 tell)
{
	end_offset = xy_to_offset(x, y, tell);
}

quint64 Selection::xy_to_offset(quint64 x, quint64 y, quint64 tell)
{
	return tell + (columns * y) + x;
}

bool Selection::in_range(quint64 x, quint64 y, quint64 tell)
{
	quint64 test = xy_to_offset(x, y, tell);
	quint64 s = start_offset;
	quint64 e = end_offset;

	/* user was selecting 'up' instead of 'down' */
	if (e < s) {
		quint64 tmp = s;
		s = e;
		e = tmp;
	}

	if (test >= s && test <= e) return true;
	return false;
}
