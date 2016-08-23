#include "structtypes.h"
#include <QList>

unsigned int StructStatement::record_length()
{
    unsigned int mysize = 0;
    for (QList<StructStatement*>::Iterator i = statements.begin(); i != statements.end(); i++)
        mysize += (*i)->record_length();
    return mysize;
}

unsigned int FloatStatement::record_length()
{
    unsigned int mysize = (sign_bits * exponent_bits * mantissa_bits);
    mysize += StructStatement::record_length();
    return mysize;
}
