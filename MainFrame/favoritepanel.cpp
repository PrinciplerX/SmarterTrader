#include "favoritepanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include <QSplitter>
#include <QDebug>
#include <QTabBar>
#include <historydatamanager.h>
#include <tickdatamanager.h>
#include <marketinfo.h>
#include <plotablecurvefactory.h>
#include <realtimevolume.h>

FavoritePanel::FavoritePanel(FavoriteListModel *model, QWidget *parent) :
    QTabWidget(parent),
    treeView(new QTreeView()),
    listModel(model),
    itemPlot(new RealTimeCompositePlot()),
    indexPlot(new RealTimeCompositePlot())
{
    this->treeView->setModel(model);

    tabBar()->setObjectName(QString("PanelTabBar"));

    treeView->header()->setMinimumSectionSize(60);
    treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView->header()->setStretchLastSection(false);
    treeView->setUniformRowHeights(true);//避免计算行高

    treeView->setRootIsDecorated(false);
    treeView->header()->setSortIndicatorShown(true);

    QTabBar* tabBar = new QTabBar();
    tabBar->setShape(QTabBar::TriangularSouth);
    tabBar->setExpanding(false);
    tabBar->addTab(QString("自选列表"));
    tabBar->addTab(QString("我的持仓"));
    tabBar->addTab(QString("账户分析"));
    tabBar->addTab(QString("最近浏览"));
    tabBar->setObjectName(QString("ChoiceTabBar"));
    QFont f = tabBar->font();
    f.setPointSize(9);
    tabBar->setFont(f);

    QWidget* listWidget = new QWidget();
    QVBoxLayout* itemLayout=new QVBoxLayout(listWidget);
    itemLayout->setSpacing(0);
    itemLayout->setMargin(0);
    itemLayout->addWidget(treeView);
    itemLayout->addWidget(tabBar);

    QTabWidget* detailWidget = new QTabWidget();
    detailWidget->setTabPosition(QTabWidget::South);
    detailWidget->tabBar()->setObjectName(QString("ChoiceTabBar"));
    detailWidget->tabBar()->setExpanding(false);
    detailWidget->setFont(f);
    detailWidget->setTabShape(QTabWidget::Triangular);

    QSplitter* plotWidget = new QSplitter(Qt::Vertical);
    plotWidget->addWidget(itemPlot);
    plotWidget->addWidget(indexPlot);
    plotWidget->setHandleWidth(1);
    plotWidget->setMinimumWidth(240);
    detailWidget->addTab(plotWidget, QString("统计"));

    StatisticPanel* sPanel = new StatisticPanel();

    QSplitter* itemWidget = new QSplitter(Qt::Horizontal);
    itemWidget->setHandleWidth(1);
    itemWidget->addWidget(listWidget);
    itemWidget->addWidget(detailWidget);

    this->addTab(itemWidget, QString("自选列表"));
    this->addTab(sPanel, QString("我的持仓"));
    this->addTab(new QWidget(), QString("账户分析"));
    this->addTab(new QWidget(), QString("最近浏览"));

    connect(treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onTreeViewItemDoubleClicked(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(onTreeViewItemClicked(QModelIndex)));
}

void FavoritePanel::addItemSlot(){
}

void FavoritePanel::deleteItemSlot(){
}

void FavoritePanel::editSlot(){
}

void FavoritePanel::onTreeViewItemDoubleClicked(QModelIndex index) {
    emit itemDoubleClicked(listModel->getSymbol(index));
}

void FavoritePanel::onTreeViewItemClicked(QModelIndex index)
{
    QString symbol = listModel->getSymbol(index);

    itemPlot->clearPlotableItem();
    indexPlot->clearPlotableItem();

    AbstractHistoryData* h1Data = TickDataManager::instance().getRealTimeTradeData(symbol, QDate::currentDate().toString("yyyy-MM-dd"));
    AbstractHistoryData* h2Data = TickDataManager::instance().getRealTimeTradeData(symbol, QDate::currentDate().toString("yyyy-MM-dd"));

    PlotableCurve* close1 = PlotableCurveFactory::createCLOSE(h1Data);
    close1->setPen(QPen(Qt::white));
    itemPlot->addPricePlotItem(close1);
    RealTimeVolume* v1 = new RealTimeVolume(h1Data);
    h1Data->attachIndicator(v1);
    itemPlot->addVolumePlotItem(v1);
    itemPlot->setData(h1Data);

    PlotableCurve* close2 = PlotableCurveFactory::createCLOSE(h2Data);
    close2->setPen(QPen(Qt::white));
    indexPlot->addPricePlotItem(close2);
    RealTimeVolume* v2 = new RealTimeVolume(h2Data);
    h2Data->attachIndicator(v2);
    indexPlot->addVolumePlotItem(v2);
    indexPlot->setData(h2Data);
}
