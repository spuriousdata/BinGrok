#ifndef TRTABLE_H
#define TRTABLE_H

#include <QObject>
#include <QHash>
#include <QString>

class TrTable : public QObject
{
    Q_OBJECT
public:
    explicit TrTable(QObject *parent = 0);
    inline QString get_hex(const quint8 b) { return b2s[b]; }
    inline quint8 get_byte(const QString & s ) { return s2b[s]; }
    inline QChar get_char(const quint8 b) { return b2c[b]; }

private:
	QString b2s[256];
    char b2c[256];
	QHash<const QString, quint8> s2b;

	const QString hex(const quint8);
    char hex_nybble(const quint8);
	/*
	const quint8 byte(const QString &);
	const quint8 byte_nybble(const char c);
	*/

signals:

public slots:

};

#endif // TRTABLE_H
