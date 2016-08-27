#ifndef STRUCTTYPES_H_
#define STRUCTTYPES_H_

#include <QString>
#include <QList>
#include <cstring>

class StructStatement
{
public:
    QString name, type;
    char *data = nullptr;

    StructStatement() {}
    StructStatement(QString n, QString t) : name(n), type(t) {}
    virtual ~StructStatement() {}
    virtual QString get_name();
    unsigned int virtual record_length() { return 0; }
    void alloc();
    void set_data(const char *d);
};

class NumericStatement : public StructStatement
{
public:
    unsigned int precision = 0;
    bool is_unsigned;

    NumericStatement() : StructStatement() {}
    NumericStatement(QString n, QString t) : StructStatement(n, t) { }
    virtual QString get_name();
    unsigned int record_length() { return precision; }
};

class StringStatement : public StructStatement
{
public:
    unsigned int length_bytes = 0;
    QString length_backref;

    StringStatement() : StructStatement() {}
    StringStatement(QString n, QString t) : StructStatement(n, t) { }
    virtual QString get_name();
    unsigned int record_length() { return (length_bytes); }
};

class Struct : public StructStatement
{
public:
    QList<StructStatement *> statements;

    Struct() : StructStatement() {}
    Struct(QString n, QString t) : StructStatement(n, t) { }
    virtual ~Struct() { for (auto it = statements.begin(); it != statements.end(); it++) delete *it; }
    virtual QString get_name();
    void populate(const char *d);
    unsigned int record_length() { unsigned int s = 0; for (auto it = statements.begin(); it != statements.end(); it++) s += (*it)->record_length(); return s;}
};

#endif /* STRUCTTYPES_H_ */
