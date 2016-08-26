#ifndef RD_PARSER_H
#define RD_PARSER_H

#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <QList>
#include <QException>
#include <initializer_list>
#include "structtypes.h"


typedef enum {
    identifier,
    lcurly,
    rcurly,
    lparen,
    rparen,
    lbracket,
    rbracket,
    number,
    stringsym,
    arraysym,
    structsym,
    intsym,
    uintsym,
    floatsym,
    semicolon,
    comma,
    colon,
    theend,
    unknown
} Symbol;

class RDParser
{
public:
    struct {
        Symbol symbol;
        QString token;
    } next;
    QString data;
    QTextStream *datastream;


    Struct *parse(const QString &input);

    class ParserException : public QException
    {
    public:
        QString message;

        ParserException() : QException () {}
        void set_message(QString m);
        void raise() const;
        ParserException *clone() const { return new ParserException(*this); }
    };

private:
    QRegularExpression number_re = QRegularExpression("\\d+");
    QRegularExpression ident_re  = QRegularExpression("^[a-zA-Z_]\\w+");

    QChar peek();
    Symbol peek_symbol();
    bool expect(Symbol s, QString *ret = nullptr);
    bool accept(Symbol s, QString *ret = nullptr);
    bool at_boundary(QChar c);
    bool one_of(std::initializer_list<Symbol> s);
    void nextsym();
    Struct *_struct();
    void block(Struct *container);
    void statements(QList<StructStatement *> *container);
    StructStatement *statement();
    void precision(NumericStatement *container = nullptr);
    void string_length(StringStatement *container);
    void float_precision(FloatStatement *container = nullptr);
    Symbol string_to_symbol(const QString & token);
};

bool is_word_char(QChar c);

#endif
