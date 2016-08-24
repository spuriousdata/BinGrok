#include <QString>
#include <QTextStream>
#include <QRegularExpression>
#include <initializer_list>
#include "rd_parser.h"

#ifndef QT_NO_DEBUG
#include <QtDebug>
#include <QString>
#endif

void RDParser::parse(const QString &input)
{
    data = QString(input);
    datastream = new QTextStream(&data);
    nextsym();
    _struct();
}

void RDParser::_struct()
{
    expect(structsym);
    block();
    expect(identifier);
    expect(semicolon);
}

void RDParser::block()
{
    expect(lcurly);
    statements();
    expect(rcurly);
}

void RDParser::statements()
{
    while (current.symbol != rcurly)
        statement();
}

void RDParser::precision()
{
    bool p;

    expect(lparen);
    p = accept(number);
    if (!p) {
        expect(colon);
        expect(identifier);
    }
    expect(rparen);
}

void RDParser::float_precision()
{
    expect(lparen);
    expect(number);
    expect(comma);
    expect(number);
    expect(comma);
    expect(number);
    expect(rparen);
}

bool RDParser::one_of(std::initializer_list<Symbol> s)
{
    for (auto it = s.begin(); it != s.end(); it++)
        if (accept(*it))
            return true;
    return false;
}

void RDParser::statement()
{
    if (one_of({intsym, uintsym, charsym, ucharsym, stringsym})) {
        precision();
        expect(identifier);
        expect(semicolon);
    } else if (accept(floatsym)) {
        float_precision();
        expect(identifier);
        expect(semicolon);
    } else {
        _struct();
    }
}

bool RDParser::accept(Symbol s)
{
    if (current.symbol == s) {
        nextsym();
        return true;
    }
    return false;
}

bool RDParser::expect(Symbol s)
{
    if (accept(s))
        return true;
    throw new RDParser::ParserException("Unknown symbol.");
}

QChar RDParser::peek()
{
    QString c;
    qint64 p = datastream->pos();
    c = datastream->read(1);
    datastream->seek(p);
    return c.at(0);
}

// XXX: This sucks
Symbol RDParser::peek_symbol()
{
    Symbol s,n;
    QString t;
    qint64 p;

    // save state
    s = current.symbol;
    t = current.token;
    p = datastream->pos();

    nextsym();

    n = current.symbol;

    // reset state
    current.symbol = s;
    current.token = t;
    datastream->seek(p);

    return n;
}

bool RDParser::at_boundary(QChar c)
{
    QChar n = peek();
    return (!((is_word_char(c) == is_word_char(n)) && (c.isSpace() == n.isSpace())) || (c.isPunct() && n.isPunct()));
}

bool is_word_char(QChar c)
{
    return (c.isLetterOrNumber() || c == '_');
}

Symbol RDParser::string_to_symbol(const QString & token)
{
    QRegularExpression number_re = QRegularExpression("\\d+");
    QRegularExpression ident_re  = QRegularExpression("^[a-zA-Z_]\\w+");

    if (token == "")
        return theend;
    if (token == "struct")
        return structsym;
    if (token == "{")
        return lcurly;
    if (token == "}")
        return rcurly;
    if (token == "(")
        return lparen;
    if (token == ")")
        return rparen;
    if (token == "int")
        return intsym;
    if (token == "uint")
        return uintsym;
    if (token == "char")
        return charsym;
    if (token == "uchar")
        return ucharsym;
    if (token == "float")
        return floatsym;
    if (token == "string")
        return stringsym;
    if (token == ";")
        return semicolon;
    if (token == ",")
        return comma;
    if (token == ":")
        return colon;
    if (number_re.match(token).hasMatch())
        return number;
    if (ident_re.match(token).hasMatch())
        return identifier;
    return unknown;
}

void RDParser::nextsym()
{
    QChar c;
    QString token;

    *datastream >> c;
    if (!datastream->atEnd()) {
        // eat whitespace
        while (c.isSpace())
            *datastream >> c;
    }

    while (!datastream->atEnd()) {
        token.append(c);
        if (at_boundary(c))
            break;
        *datastream >> c;
    }

    if (!c.isNull() && token.isEmpty())
        token.append(c);

    current.symbol = string_to_symbol(token);
    current.token = token;
}
