#ifndef NEWSTESTWIDGET_H
#define NEWSTESTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <newsmanager.h>
#include <tradedata.h>
#include "newswidget.h"

class NewsTestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewsTestWidget(QWidget *parent = 0);

protected slots:
    void onNavigationListActivated(int index);
    void refreshData();
    void onTimer();
    void onNewsItemLoaded();

private:
    //! 防止网络请求速度过快的定时器。
    QTimer* mTimer;
    QListWidget* mNavigationList;
    QStackedWidget* mContentWidget;
    QPushButton* mActionButton;
    bool mIsDownloading;
};

#endif // NEWSTESTWIDGET_H
