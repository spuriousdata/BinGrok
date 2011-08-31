#include "selection.h"
#include <QPoint>

bool Selection::in_range(int col, int row)
{
	QPoint c = QPoint(col, row);

	if (_start.manhattanLength() <= c.manhattanLength() &&
			_end.manhattanLength() >= c.manhattanLength())
		return true;
	return false;
}
