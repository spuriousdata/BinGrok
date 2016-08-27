#include "structtypes.h"

QString StructStatement::get_name()
{
    return name;
}

void StructStatement::alloc()
{
    data = new char[record_length()]();
}

void StructStatement::set_data(const char *d)
{
    if (data == nullptr)
        alloc();
    std::memcpy(data, d, record_length());
}

void Struct::populate(const char * const d)
{
    char *ptr = const_cast<char* const>(d);

    for (auto it = statements.begin(); it != statements.end(); it++) {
        Struct *strct;
        if ((strct = dynamic_cast<Struct*>(*it)) != NULL) {
            strct->populate(ptr);
        } else {
            (*it)->set_data(ptr);
            ptr += (*it)->record_length();
        }
    }
}


QString NumericStatement::get_name()
{
    return name;
}

QString StringStatement::get_name()
{
    return name;
}

QString Struct::get_name()
{
    return name;
}

