#ifndef SELECTION_H
#define SELECTION_H

#include <QPoint>

class Selection
{
public:
    Selection(uint columns, quint64 size) : columns(columns),size(size) { reset(); }

    void resize(uint c) { columns = c; }
    void resize(uint c, quint64 s) { size = s; resize(c); }
    void reset() { start_offset = end_offset = 0; active = false;}

	void start(int x, int y, quint64 tell);
	void start(QPoint p, quint64 tell) { start(p.x(), p.y(), tell); }
    void start(quint64 tell) { start_offset = tell; active = true; }

	void end(int x, int y, quint64 tell);
	void end(QPoint p, quint64 tell) { end(p.x(), p.y(), tell); }
    void end(quint64 tell) { end_offset = tell; active = true;}

    void select(quint64 s, quint64 e) { start(s); end(e); active=true; }

	bool in_range(int x, int y, quint64 tell);
    bool in_range(quint64 tell);
    bool is_active() { return active; }

private:
    uint columns;
    quint64 start_offset, end_offset, size;
    bool active = false;

	quint64 xy_to_offset(int x, int y, quint64 tell);
};

#endif // SELECTION_H
