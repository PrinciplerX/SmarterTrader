#include "realtimepanel.h"
#include <QHBoxLayout>
#include <realtimevolume.h>
#include <marketinfo.h>
#include "tradedatahelper.h"
#include "splitterplot.h"
#include "kdj.h"
#include <realtimedatamanager.h>
#include <QTabBar>
#include <QTabWidget>
#include "plotablecurvefactory.h"
#include "historydatamanager.h"

using TradingCurveWidgets::SplitterPlot;

RealTimePanel::RealTimePanel(QWidget *parent):
    QWidget(parent),
    tickTable(new TickTable()),
    restingOrderTable(new RestingOrderTable()),
    realTimeCompositePlot(new RealTimeCompositePlot()),
    stackedWidget(new QStackedWidget()),
    quoteLabel(new QuotationLabel())

{
    QFont dataFont = this->font();
    dataFont.setPointSize(10);
    this->setFont(dataFont);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* detailLayout = new QVBoxLayout();
    QVBoxLayout* realTimePlotLayout = new QVBoxLayout();
    QSplitter* leftSplitter = new QSplitter(Qt::Vertical);

    leftSplitter->insertWidget(0, realTimeCompositePlot);
    leftSplitter->insertWidget(1, stackedWidget);
    leftSplitter->setHandleWidth(1);
    leftSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* realTimePage = new QWidget();
    realTimePage->setLayout(detailLayout);
    detailLayout->setMargin(0);
    detailLayout->setSpacing(1);

    QTabBar* tabBar = new QTabBar();
    tabBar->setObjectName(QString("ChoiceTabBar"));
    tabBar->setExpanding(false);
    tabBar->setShape(QTabBar::TriangularSouth);
    tabBar->addTab(QString("分时量"));
    tabBar->addTab(QString("指标"));
    tabBar->addTab(QString("关联报价"));
    tabBar->addTab(QString("资讯"));

    realTimePlotLayout->addWidget(leftSplitter);
    realTimePlotLayout->addWidget(tabBar);
    realTimePlotLayout->setMargin(0);
    realTimePlotLayout->setSpacing(1);

    QFrame* realTimeFrame = new QFrame();
    realTimeFrame->setFrameShape(QFrame::StyledPanel);
    realTimeFrame->setLayout(realTimePlotLayout);

    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->setFixedWidth(240);
    tabWidget->tabBar()->setExpanding(false);
    tabWidget->setTabShape(QTabWidget::Triangular);
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->addTab(realTimePage, QString("分笔"));
    tabWidget->addTab(new QWidget(), QString("指数"));
    tabWidget->addTab(new QWidget(), QString("统计"));
    tabWidget->addTab(new QWidget(), QString("财务"));
    tabWidget->tabBar()->setObjectName(QString("ChoiceTabBar"));

    mainLayout->addWidget(realTimeFrame);
    mainLayout->addWidget(tabWidget);
    mainLayout->setSpacing(1);
    mainLayout->setMargin(0);

    detailLayout->addWidget(quoteLabel);
    detailLayout->addWidget(restingOrderTable);
    detailLayout->addWidget(tickTable);
}

void RealTimePanel::setData(const AbstractRealTimeData *data)
{
    if(mData != nullptr){
        disconnect(mData, SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));
    }
    mData = data;
    connect(mData, SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));
}

void RealTimePanel::setTickData(const AbstractTickData *tickData)
{
    if(mTickData != nullptr){
        disconnect(mTickData, SIGNAL(dataLoaded()), this, SLOT(onTickDataLoaded()));
    }
    mTickData = tickData;
    connect(mTickData, SIGNAL(dataLoaded()), this, SLOT(onTickDataLoaded()));
}

void RealTimePanel::onDataUpdated()
{
    quoteLabel->updateData(mData->getQuoteData());
    restingOrderTable->updateData(mData->getQuoteData());
}

void RealTimePanel::onTickDataLoaded()
{
    if(mTickData->size() == 0) return;

    realTimeCompositePlot->clearPlotableItem();

    AbstractHistoryData* h1Data = HistoryDataManager::instance().fromTickData(mTickData,
                                                                              MarketInfo::instance().tradeHour(MarketInfo::A_Shares));

    DateSeries series = TradeDataHelper::getDateSeries(h1Data->data());

    TickModel* model = new TickModel();
    model->setData(mTickData->data());
    tickTable->setModel(model);

    PlotableCurve* close = PlotableCurveFactory::createCLOSE(h1Data);
    close->setPen(QPen(Qt::white));
    realTimeCompositePlot->addPricePlotItem(close);
    realTimeCompositePlot->addVolumePlotItem(new RealTimeVolume(h1Data));
    realTimeCompositePlot->setTracerLabels(series);
    realTimeCompositePlot->setBaseValue(h1Data->at(0).open);
    realTimeCompositePlot->updateAxes();
    realTimeCompositePlot->replot();
}

