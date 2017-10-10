#ifndef HIERARCHICTABWIDGET_H
#define HIERARCHICTABWIDGET_H

#include <QWidget>
#include <QTabBar>
#include <QStackedLayout>

class HierarchicTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HierarchicTabWidget(QWidget *parent = 0);

signals:

public slots:

private:
    QStackedLayout* mContentLayout;

};

#endif // HIERARCHICTABWIDGET_H
