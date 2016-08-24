#ifndef RD_PARSER_H
#define RD_PARSER_H

#include <QString>
#include <QTextStream>
#include <initializer_list>


typedef enum {
    theend,
    identifier,
    number,
    stringsym,
    structsym,
    lcurly,
    rcurly,
    lparen,
    rparen,
    intsym,
    uintsym,
    charsym,
    ucharsym,
    floatsym,
    semicolon,
    comma,
    colon,
    unknown
} Symbol;

class RDParser
{
public:
    struct {
        Symbol symbol;
        QString token;
    } current;
    QString data;
    QTextStream *datastream;


    void parse(const QString &input);

    class ParserException
    {
    public:
        QString message;

        ParserException(QString m) { message = m; }

    };

private:
    QChar peek();
    Symbol peek_symbol();
    bool at_boundary(QChar c);
    bool expect(Symbol s);
    bool accept(Symbol s);
    bool one_of(std::initializer_list<Symbol> s);
    void nextsym();
    void _struct();
    void block();
    void statements();
    void statement();
    void precision();
    void float_precision();
    Symbol string_to_symbol(const QString & token);
};

bool is_word_char(QChar c);

#endif
