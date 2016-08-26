#include "rd_parser.h"

#ifndef QT_NO_DEBUG
#include <QtDebug>
#endif

Struct *RDParser::parse(const QString &input)
{
    data = QString(input);
    datastream = new QTextStream(&data);
    nextsym();
    return _struct();
}

Struct *RDParser::_struct()
{
    Struct *s = new Struct();
    s->type = "struct";

    expect(structsym);
    block(s);
    expect(identifier, &(s->name));
    expect(semicolon);

    return s;
}

void RDParser::block(Struct *container)
{
    expect(lcurly);
    statements(&(container->statements));
    expect(rcurly);
}

void RDParser::statements(QList<StructStatement *> *container)
{
    while (next.symbol != rcurly) {
        container->append(statement());
    }
}

void RDParser::precision(NumericStatement *container)
{
    QString val;

    expect(lparen);
    accept(number, &val);
    container->precision_bits = val.toUInt();
    expect(rparen);
}

void RDParser::string_length(StringStatement *container)
{
    QString bytes;
    expect(lbracket);
    if (accept(colon)) {
        expect(identifier, &container->length_backref);
    } else {
        expect(number, &bytes);
        container->length_bytes = bytes.toUInt();
    }
    expect(rbracket);
}

void RDParser::float_precision(FloatStatement *container)
{
    QString val;

    expect(lparen);
    expect(number, &val);
    container->sign_bits = val.toUInt();
    expect(comma);
    expect(number, &val);
    container->exponent_bits = val.toUInt();
    expect(comma);
    expect(number, &val);
    container->mantissa_bits = val.toUInt();
    expect(rparen);
}

bool RDParser::one_of(std::initializer_list<Symbol> s)
{
    for (auto it = s.begin(); it != s.end(); it++)
        if (accept(*it))
            return true;
    return false;
}

StructStatement *RDParser::statement()
{
    QString name;
    StructStatement *ss = new StructStatement();
    bool try_struct = false;

    if (accept(intsym)) {
        ss = new NumericStatement();
        precision(static_cast<NumericStatement*>(ss));
        expect(identifier, &name);
        expect(semicolon);
    } else if (accept(uintsym)) {
        ss = new NumericStatement();
        precision(static_cast<NumericStatement*>(ss));
        static_cast<NumericStatement*>(ss)->is_unsigned = true;
        expect(identifier, &name);
        expect(semicolon);
    } else if (one_of({stringsym, arraysym})) {
        ss = new StringStatement();
        string_length(static_cast<StringStatement*>(ss));
        expect(identifier, &name);
        expect(semicolon);
    } else if (accept(floatsym)) {
        ss = new FloatStatement();
        float_precision(static_cast<FloatStatement*>(ss));
        expect(identifier, &name);
        expect(semicolon);
    } else {
       try_struct = true;
    }

    if (try_struct) {
        return _struct();
    } else {
        ss->name = name;
        return ss;
    }
}

bool RDParser::accept(Symbol s, QString *ret)
{
    if (next.symbol == s) {
        if (ret != nullptr)
            *ret = next.token;
        nextsym();
        return true;
    }
    return false;
}

bool RDParser::expect(Symbol s, QString *ret)
{
    if (accept(s, ret))
        return true;
    QString message;
    QTextStream(&message) << "Got '" << next.token << "' expected '" << s << "'";
    auto ex = RDParser::ParserException();
    ex.set_message(message);
    throw ex;
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
    s = next.symbol;
    t = next.token;
    p = datastream->pos();

    nextsym();

    n = next.symbol;

    // reset state
    next.symbol = s;
    next.token = t;
    datastream->seek(p);

    return n;
}

bool RDParser::at_boundary(QChar c)
{
    QChar n = peek();
    /* each punct symbol should be treated separately, so we treat them as word boundaries */
    return (!((is_word_char(c) == is_word_char(n)) && (c.isSpace() == n.isSpace())) || c.isPunct() || n.isPunct());
}

bool is_word_char(QChar c)
{
    return (c.isLetterOrNumber() || c == '_');
}

Symbol RDParser::string_to_symbol(const QString & token)
{


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
    if (token == "[")
        return lbracket;
    if (token == "]")
        return rbracket;
    if (token == "int")
        return intsym;
    if (token == "uint")
        return uintsym;
    if (token == "float")
        return floatsym;
    if (token == "string")
        return stringsym;
    if (token == "array")
        return arraysym;
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

    next.symbol = string_to_symbol(token);
    next.token = token;
}

void RDParser::ParserException::raise() const
{
    throw *this;
}

void RDParser::ParserException::set_message(QString m)
{
    message = m;
}
