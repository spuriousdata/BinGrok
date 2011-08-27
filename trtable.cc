#include "trtable.h"
#include <QHash>
#include <QString>

TrTable::TrTable(QObject *parent) :
    QObject(parent)
{
	for (int i = 0; i <= 255; i++) {
		b2s[i] = hex(i);
		s2b.insert(b2s[i], i);
	}

	// Shrink to 256 slots
	s2b.squeeze();
}

const QString TrTable::hex(const quint8 b)
{
	char h[3] = { hex_nybble((b & 0xF0) >> 4), hex_nybble(b & 0x0F), '\0' };
	return QString(h);
}

const char TrTable::hex_nybble(const quint8 n)
{
	if (n >= 0 && n <= 9) return n + '0';
	return (n - 10) + 'A';
}

/*
const quint8 TrTable::byte(const QString & s)
{
	return (byte_nybble(s.at(0).toAscii()) << 4 |
			byte_nybble(s.at(1).toAscii()));
}

const quint8 TrTable::byte_nybble(const char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	return (c - 'A') + 10;
}
*/
