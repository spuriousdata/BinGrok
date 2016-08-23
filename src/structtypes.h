#ifndef STRUCTTYPES_H_
#define STRUCTTYPES_H_

#include <string>
#include <QList>

class StructStatement
{
public:
    QList<StructStatement *> statements;
    std::string type, name;
    virtual ~StructStatement() {}

    unsigned int virtual record_length();
    inline unsigned int record_length_bytes() { return record_length() / 8; }
};

class FloatStatement : StructStatement
{
public:
    unsigned int sign_bits = 0;
    unsigned int exponent_bits = 0;
    unsigned int mantissa_bits = 0;

    unsigned int record_length();
};

class NumericStatement : StructStatement
{
public:
    unsigned int precision_bits = 0;

    unsigned int record_length();
};

#endif /* STRUCTTYPES_H_ */
