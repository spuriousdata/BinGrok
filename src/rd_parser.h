#include <string>
#include <QException>


typedef enum {
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
    comma
} Symbol;

class RDParser
{
public:
    Symbol symbol;
    std::string data;
    std::string token;
    unsigned long offset = 0;

    void parse(std::string & input);

    class ParserException : QException
    {
    public:
        ParserException(QString m) : message(m) {}
        QString message;
    };

private:
    int expect(Symbol s);
    int accept(Symbol s);
    std::string nextsym();
    void _struct();
    void block();
    void statements();
};
