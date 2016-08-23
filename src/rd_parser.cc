#include <string>
#include "rd_parser.h"

#ifndef QT_NO_DEBUG
#include <QtDebug>
#include <QString>
#endif

void RDParser::parse(std::string & input)
{
	data = input;

    nextsym();
    _struct();
}

void RDParser::_struct()
{
    expect(structsym);
    block();
}

void RDParser::block()
{
    expect(lcurly);
    statements();
    expect(rcurly);
    expect(identifier);
    expect(semicolon);
}

void RDParser::statements()
{
    if (accept(intsym) || accept(uintsym) || accept(charsym) || accept(ucharsym) || accept(stringsym)) {
        expect(lparen);
        expect(number);
        expect(rparen);
    } else if (accept(floatsym)) {
        expect(lparen);
        expect(number);
        expect(comma);
        expect(number);
        expect(comma);
        expect(number);
        expect(rparen);
    } else {
        _struct();
    }
}

int RDParser::accept(Symbol s)
{
    if (symbol == s) {
        nextsym();
        return 1;
    }
    return 0;
}

int RDParser::expect(Symbol s)
{
    if (accept(s))
        return 1;
    throw new RDParser::ParserException("Unknown symbol.");
    return 0;
}

std::string RDParser::nextsym()
{
    unsigned long next_space;
#ifndef QT_NO_DEBUG
    qDebug() << "Offset: " << offset << "Length: " << data.length();
#endif
	if (offset >= data.length()) {
        return "";
	}

    next_space = data.find_first_of(" \t\n", offset);

    token = data.substr(offset, next_space-offset);

    offset += next_space + 1;
#ifndef QT_NO_DEBUG
    qDebug() << "Token: " << QString().fromStdString(token);
#endif 
    return token;
}
