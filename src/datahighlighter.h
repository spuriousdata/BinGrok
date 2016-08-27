#ifndef DATAHIGHLIGHTER_H
#define DATAHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QVector>
#include <QRegExp>
#include <QTextCharFormat>
#include <QString>
#include <QTextDocument>


class DataHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    DataHighlighter(QTextDocument *parent=0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlighting_rules;
};

#endif // DATAHIGHLIGHTER_H
