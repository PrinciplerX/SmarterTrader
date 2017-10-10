#include "newswidget.h"
#include <QListView>
#include <QTreeView>
#include <QHeaderView>
#include <QDesktopServices>

NewsWidget::NewsWidget(QTabWidget *parent):
    QTabWidget(parent),
    newsSiteWidget(new NewsSiteWidget()),
    latestNewsList(new QTreeView())
{
    tabBar()->setObjectName(QString("PanelTabBar"));

    this->tabBar()->setFixedHeight(32);

    NewsManager::instance().updateLatest(200);
    latestNewsList->header()->setMinimumSectionSize(40);
    latestNewsList->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    latestNewsList->setUniformRowHeights(true);//避免计算行高

    connect(latestNewsList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onNewsItemDoubleClicked(QModelIndex)));

    latestNewsList->setRootIsDecorated(false);
    latestNewsList->header()->setSortIndicatorShown(true);
    latestNewsList->setModel(new NewsModel(NewsManager::instance().latestNews()));

    this->addTab(latestNewsList,tr("资讯首页"));
    this->addTab(new QWidget(),tr("财经头条"));
    this->addTab(new QWidget(),tr("公司新闻"));
    this->addTab(new QWidget(),tr("24小时滚动"));
    this->addTab(new QWidget(),tr("全球市场"));
    this->addTab(new QWidget(),tr("自选股资讯"));
    this->addTab(newsSiteWidget,tr("新闻网站"));
}

void NewsWidget::onNewsItemDoubleClicked(const QModelIndex &index)
{
    QDesktopServices::openUrl(((NewsReference*)index.internalPointer())->url);
}
