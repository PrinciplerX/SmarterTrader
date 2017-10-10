#ifndef NEWSSITEWIDGET_H
#define NEWSSITEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include "newssitelistitem.h"
#include "mainframe_global.h"

class NewsSiteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewsSiteWidget(QWidget *parent = 0);

signals:

public slots:

private:
    void initNewsList();
    QListWidget* newsSiteList;
    QStackedWidget* stackedWidget;
};

#endif // NEWSSITEWIDGET_H
