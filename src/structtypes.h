#ifndef STRUCTTYPES_H
#define STRUCTTYPES_H

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
    virtual ~StructStatement() { if (data != nullptr) delete data;}
    unsigned int virtual record_length() { return 0; }
    void alloc();
    virtual void set_data(const char *d);
    virtual QString to_string(int indent_level=0) { return ""; }
};

class NumericStatement : public StructStatement
{
public:
    unsigned int precision = 0;
    bool is_unsigned;

    NumericStatement() : StructStatement() {}
    NumericStatement(QString n, QString t) : StructStatement(n, t) { }
    virtual QString to_string(int indent_level=0);
    unsigned int record_length() { return precision; }
};

class StringStatement : public StructStatement
{
public:
    unsigned int length_bytes = 0;
    QString length_backref;

    StringStatement() : StructStatement() {}
    StringStatement(QString n, QString t) : StructStatement(n, t) { }
    virtual QString to_string(int indent_level=0);
    unsigned int record_length() { return (length_bytes); }
};

class ArrayStatement : public StringStatement
{
public:
    ArrayStatement() : StringStatement() {}
    ArrayStatement(QString n, QString t) : StringStatement(n, t) {}
    virtual QString to_string(int indent_level=0);
};

class Struct : public StructStatement
{
public:
    QList<StructStatement *> statements;

    Struct() : StructStatement() {}
    Struct(QString n, QString t) : StructStatement(n, t) { }
    virtual ~Struct() { for (auto it = statements.begin(); it != statements.end(); it++) delete *it; }
    virtual QString to_string(int indent_level=0);
    void populate(const char *d);
    unsigned int record_length() { unsigned int s = 0; for (auto it = statements.begin(); it != statements.end(); it++) s += (*it)->record_length(); return s;}
};

#endif /* STRUCTTYPES_H */
