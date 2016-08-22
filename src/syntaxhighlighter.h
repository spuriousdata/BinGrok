#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QVector>
#include <QRegExp>
#include <QTextCharFormat>
#include <QString>
#include <QTextDocument>


class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument *parent=0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlighting_rules;

    QRegExp comment_start_expression;
    QRegExp comment_end_expression;

    QTextCharFormat keyword_format;
    QTextCharFormat single_line_comment_format;
    QTextCharFormat multiline_comment_format;
    QTextCharFormat integer_format;
};

#endif // SYNTAXHIGHLIGHTER_H
