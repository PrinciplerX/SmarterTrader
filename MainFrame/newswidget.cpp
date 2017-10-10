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
    latestNewsList->setUniformRowHeights(true);//��������и�

    connect(latestNewsList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onNewsItemDoubleClicked(QModelIndex)));

    latestNewsList->setRootIsDecorated(false);
    latestNewsList->header()->setSortIndicatorShown(true);
    latestNewsList->setModel(new NewsModel(NewsManager::instance().latestNews()));

    this->addTab(latestNewsList,tr("��Ѷ��ҳ"));
    this->addTab(new QWidget(),tr("�ƾ�ͷ��"));
    this->addTab(new QWidget(),tr("��˾����"));
    this->addTab(new QWidget(),tr("24Сʱ����"));
    this->addTab(new QWidget(),tr("ȫ���г�"));
    this->addTab(new QWidget(),tr("��ѡ����Ѷ"));
    this->addTab(newsSiteWidget,tr("������վ"));
}

void NewsWidget::onNewsItemDoubleClicked(const QModelIndex &index)
{
    QDesktopServices::openUrl(((NewsReference*)index.internalPointer())->url);
}
