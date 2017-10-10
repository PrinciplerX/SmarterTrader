#ifndef FORMULAHIGHLIGHTER_H
#define FORMULAHIGHLIGHTER_H

#include "tradingcurvewidgets_global.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class TRADINGCURVEWIDGETSSHARED_EXPORT FormulaHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    FormulaHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // FORMULAHIGHLIGHTER_H
