#include "selection.h"
#include <QPoint>

bool Selection::in_range(int col, int row, int columns)
{
	/* need to calculate offset for each comparator */
	int s = (columns * _start.y()) + _start.x();
	int e = (columns * _end.y()) + _end.x();
	int test = (columns * row) + col;

	/* user was selecting 'up' instead of 'down' */
	if (e < s) {
		int tmp = s;
		s = e;
		e = tmp;
	}

	if (test >= s && test <= e) return true;
	return false;
}
