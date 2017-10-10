#ifndef TITLEDLABEL_H
#define TITLEDLABEL_H

#include "expandedwidgets_global.h"
#include <QLabel>
#include <QString>

/*****************************************************************
这是一个包含标题和内容部分的Label控件。
******************************************************************/
class EXPANDEDWIDGETSSHARED_EXPORT TitledLabel : public QLabel
{
    Q_OBJECT
public:
    TitledLabel(const QString& title=QString(),
                const QString& content=QString(),
                const QString& splitter=QString(" "),
                QWidget* parent=0);

    void setTitle(const QString& title);
    QString getTitle();
    void setContent(const QString& content);
    QString getContent();

    virtual ~TitledLabel();
protected:
    virtual void setText(const QString &);

private:
    QString title;
    QString content;
    //标题和内容之间的分隔符
    QString splitter;
};

#endif // TITLEDLABEL_H
