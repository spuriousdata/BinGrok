#include "trtable.h"
#include <QHash>
#include <QString>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif


TrTable::TrTable(QObject *parent) :
    QObject(parent)
{
	for (int i = 0; i <= 255; i++) {
		b2s[i] = hex(i);
		s2b.insert(b2s[i], i);
        if (i >= 0x21 && i <= 0x7E ) {
            b2c[i] = (char)(i&0xFF);
#ifndef QT_NO_DEBUG
            qDebug() << "b2c[" << i << "]: " << (char)(i&0xFF);
#endif
        } else {
            b2c[i] = '.';
        }
	}

    // Squeeze length to 256
	s2b.squeeze();
}

const QString TrTable::hex(const quint8 b)
{
	char h[3] = { hex_nybble((b & 0xF0) >> 4), hex_nybble(b & 0x0F), '\0' };
	return QString(h);
}

char TrTable::hex_nybble(const quint8 n)
{
    if (n <= 9) return n + '0';
    else return (n - 10) + 'A';
}
