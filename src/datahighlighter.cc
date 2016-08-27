#include "datahighlighter.h"

#include <QSyntaxHighlighter>
#include <QFont>
#include <QStringList>
#include <QString>

#define BOUNDARY(s) ("\\b" s "\\b")

DataHighlighter::DataHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter (parent)
{    

    QTextCharFormat string_format, ident_format, integer_format;

    string_format.setForeground(Qt::darkBlue);

    highlighting_rules.append({
                                  QRegExp("\".*\";"),
                                  string_format
                              });

    ident_format.setForeground(Qt::blue);
    ident_format.setFontItalic(true);

    highlighting_rules.append({
                                  QRegExp(".+(?=:)"),
                                  ident_format
                              });

    integer_format.setForeground(Qt::black);
    integer_format.setFontWeight(QFont::Bold);
    highlighting_rules.append({
                                  QRegExp("\\d+"),
                                  integer_format
                              });

}

void DataHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlighting_rules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}
