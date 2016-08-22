#include "selection.h"
#include <QPoint>

#ifndef QT_NO_DEBUG
#include <QtDebug>
#endif

void Selection::start(int x, int y, quint64 tell)
{
	start_offset = xy_to_offset(x, y, tell);
	end_offset = start_offset;
#ifndef QT_NO_DEBUG
    qDebug() << "Selection::start(" << start_offset << ", " << end_offset << ")";
#endif
}

void Selection::end(int x, int y, quint64 tell)
{
	end_offset = xy_to_offset(x, y, tell);
#ifndef QT_NO_DEBUG
    qDebug() << "Selection::end(" << start_offset << ", " << end_offset << ")";
#endif
}

quint64 Selection::xy_to_offset(int x, int y, quint64 tell)
{
	return tell + (columns * y) + x;
}

bool Selection::in_range(int x, int y, quint64 tell)
{
	quint64 test = xy_to_offset(x, y, tell);
	quint64 s = start_offset;
	quint64 e = end_offset;

	if (test >= size) return false;  // can't select outside of file size

	/* user was selecting 'up' instead of 'down' */
	if (e < s) {
		quint64 tmp = s;
		s = e;
		e = tmp;
	}

	if (test >= s && test <= e) return true;
	return false;
}
