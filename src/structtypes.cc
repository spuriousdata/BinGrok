#include <QTextStream>
#include <QByteArray>
#include "structtypes.h"


/*******************************************
 *           StructStatement
 * *****************************************/
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

/*******************************************
 *                 Struct
 * *****************************************/
QString Struct::to_string(int indent_level)
{
    QString out;
    QString indent(indent_level, ' ');
    QTextStream writer(&out);

    writer  << indent << name << " {" << endl;

    for (auto it = statements.begin(); it != statements.end(); it++) {
        writer << indent << (*it)->to_string(indent_level+1);
    }
    writer << indent << "};" << endl;
    return out;
}

void Struct::populate(const char * const d)
{
    char *ptr = const_cast<char* const>(d);

    for (auto it = statements.begin(); it != statements.end(); it++) {
        Struct *strct;
        if ((strct = dynamic_cast<Struct*>(*it)) != NULL)
            strct->populate(ptr);
        else
            (*it)->set_data(ptr);
        ptr += (*it)->record_length();
    }
}

/*******************************************
 *            NumericStatement
 * *****************************************/
QString NumericStatement::to_string(int indent_level)
{
    QString out;
    QString indent(indent_level, ' ');
    QTextStream writer(&out);
    QString formatted_data(QByteArray(data, record_length()));
    while(formatted_data.startsWith('0'))
        formatted_data.remove(0, 1);

    writer << indent << name << ": " << formatted_data << ";" << endl;
    return out;
}

/*******************************************
 *           StringStatement
 * *****************************************/
QString StringStatement::to_string(int indent_level)
{
    QString out;
    QString indent(indent_level, ' ');
    QTextStream writer(&out);
    QString formatted_data(QByteArray(data, record_length()));
    formatted_data.replace("\n", "\\n").replace("\t", "\\t");

    writer << indent << name << ": \"" << formatted_data << "\";" << endl;
    return out;
}

/*******************************************
 *            ArrayStatement
 * *****************************************/
QString ArrayStatement::to_string(int indent_level)
{
    QString out;
    QString indent(indent_level, ' ');
    QTextStream writer(&out);
    QByteArray formatted_data(data, record_length());

    writer << indent << name << ": [" << formatted_data << "];" << endl;
    return out;
}

