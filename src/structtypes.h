#ifndef STRUCTTYPES_H_
#define STRUCTTYPES_H_

#include <QString>
#include <QList>

class StructStatement
{
public:
    QString name, type;

    StructStatement() {}
    StructStatement(QString n, QString t) : name(n), type(t) {}
    virtual ~StructStatement() {}
    virtual QString get_name();
    unsigned int virtual record_length() { return 0; }
    virtual inline unsigned int record_length_bytes() { return record_length() / 8; }
};

class FloatStatement : public StructStatement
{
public:
    unsigned int sign_bits = 0;
    unsigned int exponent_bits = 0;
    unsigned int mantissa_bits = 0;

    FloatStatement() : StructStatement() {}
    FloatStatement(QString n, QString t) : StructStatement(n, t) { }
    virtual QString get_name();
    unsigned int record_length() { return (sign_bits + exponent_bits + mantissa_bits) + StructStatement::record_length(); }
};

class NumericStatement : public StructStatement
{
public:
    unsigned int precision_bits = 0;
    bool is_unsigned;

    NumericStatement() : StructStatement() {}
    NumericStatement(QString n, QString t) : StructStatement(n, t) { }
    virtual QString get_name();
    unsigned int record_length() { return precision_bits + StructStatement::record_length(); }
};

class StringStatement : public StructStatement
{
public:
    unsigned int length_bytes = 0; // XXX: BYTES!!
    QString length_backref;

    StringStatement() : StructStatement() {}
    StringStatement(QString n, QString t) : StructStatement(n, t) { }
    virtual QString get_name();
    unsigned int record_length() { return (length_bytes * 8) + StructStatement::record_length(); }
    virtual inline unsigned int record_length_bytes() { return length_bytes; }
};

class Struct : public StructStatement
{
public:
    QList<StructStatement *> statements;

    Struct() : StructStatement() {}
    Struct(QString n, QString t) : StructStatement(n, t) { }
    virtual ~Struct() { for (auto it = statements.begin(); it != statements.end(); it++) delete *it; }
    virtual QString get_name();
    unsigned int record_length() { unsigned int s = 0; for (auto it = statements.begin(); it != statements.end(); it++) s += (*it)->record_length(); return s;}
};

#endif /* STRUCTTYPES_H_ */
