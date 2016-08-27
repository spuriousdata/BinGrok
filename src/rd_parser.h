#ifndef RD_PARSER_H
#define RD_PARSER_H

#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <QList>
#include <QException>
#include <QMap>
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

    RDParser();
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
    uint line;
    uint chr;
    QMap<Symbol,QString> sym_to_tok =  {
        { identifier, "<identifier>" },
        { lcurly, "{" },
        { rcurly, "}" },
        { lparen, "(" },
        { rparen, ")" },
        { lbracket, "[" },
        { rbracket, "]" },
        { number, "<number>" },
        { stringsym, "string" },
        { arraysym, "array" },
        { structsym, "struct" },
        { intsym, "int" },
        { uintsym, "uint" },
        { semicolon, ";" },
        { comma, "," },
        { colon, ":" },
        { theend, "" }, // this value needs to be blank
        { unknown, "<unknown>" }
    };
    QMap<QString,Symbol> tok_to_sym;

    QChar peek();
    Symbol peek_symbol();
    QChar read_char();
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
    Symbol string_to_symbol(const QString & token);
};

bool is_word_char(QChar c);

#endif
