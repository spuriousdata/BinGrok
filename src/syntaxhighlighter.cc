#include "syntaxhighlighter.h"

#include <QSyntaxHighlighter>
#include <QFont>
#include <QStringList>
#include <QString>

#define RE_B(s) ("\\b" s "\\b")

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter (parent)
{
    HighlightingRule rule;

    keyword_format.setForeground(Qt::darkBlue);
    keyword_format.setFontWeight(QFont::Bold);

    QStringList keyword_patterns;
    keyword_patterns << RE_B("char") << RE_B("int") << RE_B("uint") << RE_B("struct")
                     << RE_B("uchar") << RE_B("repeat") << RE_B("string") << RE_B("decimal");

    foreach (const QString &pattern, keyword_patterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keyword_format;
        highlighting_rules.append(rule);
    }

    integer_format.setForeground(Qt::green);
    rule.pattern = QRegExp("[0-9]+");
    rule.format = integer_format;
    highlighting_rules.append(rule);

    single_line_comment_format.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = single_line_comment_format;
    highlighting_rules.append(rule);

    multiline_comment_format.setForeground(Qt::red);

    comment_start_expression = QRegExp("/\\*");
    comment_end_expression = QRegExp("\\*/");
}

void SyntaxHighlighter::highlightBlock(const QString &text)
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

    int start_index = 0;
    if (previousBlockState() != 1)
        start_index = comment_start_expression.indexIn(text);

    while (start_index >= 0) {
        int endIndex = comment_end_expression.indexIn(text, start_index);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - start_index;
        } else {
            commentLength = endIndex - start_index
                            + comment_end_expression.matchedLength();
        }
        setFormat(start_index, commentLength, multiline_comment_format);
        start_index = comment_start_expression.indexIn(text, start_index + commentLength);
    }
}
