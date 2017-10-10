#include "newstestwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QTreeView>
#include <QHeaderView>

NewsTestWidget::NewsTestWidget(QWidget *parent):
    QWidget(parent),
    mTimer(new QTimer(this)),
    mNavigationList(new QListWidget()),
    mContentWidget(new QStackedWidget()),
    mActionButton(new QPushButton("更新数据"))
{
    mNavigationList->setFixedWidth(150);
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addWidget(mNavigationList);
    contentLayout->addWidget(mContentWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(mActionButton);

    mTimer->setInterval(2000);
    // 开始下载时再打开定时器。

    connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect(mActionButton, SIGNAL(clicked()), this, SLOT(refreshData()));
    connect(mNavigationList, SIGNAL(currentRowChanged(int)), this, SLOT(onNavigationListActivated(int)));
}

void NewsTestWidget::onNavigationListActivated(int index)
{
    mContentWidget->setCurrentIndex(index);
}

void NewsTestWidget::refreshData()
{
    static bool downloaded = false;
    if(!mTimer->isActive()){
        if(!downloaded){
            mTimer->start();
            downloaded = true;
        }
    }
}

void NewsTestWidget::onTimer()
{
    static int category = 0;
    if(category > 400){
        mTimer->stop();
    }
    AbstractNewsReference* item = NewsManager::instance().customNews(category, 30);
    connect(item, SIGNAL(dataLoaded()), this, SLOT(onNewsItemLoaded()));
    category++;
}

void NewsTestWidget::onNewsItemLoaded()
{
    AbstractNewsReference* item = (AbstractNewsReference*)sender();
    disconnect(item, SIGNAL(dataLoaded()), this, SLOT(onNewsItemLoaded()));
    QTreeView* newsList = new QTreeView();
    newsList->header()->setMinimumSectionSize(40);
    newsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    newsList->setUniformRowHeights(true);//避免计算行高

    newsList->setRootIsDecorated(false);
    newsList->header()->setSortIndicatorShown(true);
    newsList->setModel(new NewsModel(item));

    mContentWidget->addWidget(newsList);
    mContentWidget->setCurrentIndex(mContentWidget->count() - 1);
    mNavigationList->addItem(item->category());
}
