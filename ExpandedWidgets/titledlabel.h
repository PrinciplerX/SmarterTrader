#ifndef TITLEDLABEL_H
#define TITLEDLABEL_H

#include "expandedwidgets_global.h"
#include <QLabel>
#include <QString>

/*****************************************************************
����һ��������������ݲ��ֵ�Label�ؼ���
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
    //���������֮��ķָ���
    QString splitter;
};

#endif // TITLEDLABEL_H
