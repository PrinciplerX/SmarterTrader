#include "marketpanel.h"
#include "tradedata.h"
#include "tickdatamanager.h"
#include <QApplication>

MarketPanel::MarketPanel(QWidget *parent):
    QTabWidget(parent),
    realTimePanel(new RealTimePanel()),
    historyPanel(new HistoryPanel())
{
    tabBar()->setObjectName(QString("PanelTabBar"));

    /**************************************
    QTabBar* tabBar = new QTabBar();
    tabBar->setShape(QTabBar::RoundedSouth);
    tabBar->addTab(QString("设置"));
    tabBar->addTab(QString("指标平台"));
    tabBar->addTab(QString("资讯"));
    tabBar->addTab(QString("MACD"));
    tabBar->addTab(QString("KDJ"));
    tabBar->addTab(QString("RSI"));
    tabBar->addTab(QString("BOLL"));
    tabBar->addTab(QString("主力"));
    tabBar->addTab(QString("W&R"));
    tabBar->addTab(QString("DMI"));
    tabBar->addTab(QString("BIAS"));
    tabBar->addTab(QString("ASI"));
    tabBar->addTab(QString("VR"));
    tabBar->addTab(QString("ARBR"));
    QWidget* historyWidget = new QWidget();
    QVBoxLayout* tradingCurvePanelLayout = new QVBoxLayout(historyWidget);
    tradingCurvePanelLayout->setSpacing(0);
    tradingCurvePanelLayout->setMargin(0);
    tradingCurvePanelLayout->addWidget(tradingCurvePlotPanel);
    tradingCurvePanelLayout->addWidget(tabBar);
    *******************************************/

    //realTimePanel->setStyleSheet(QString("font:12px"));
    //tradingCurvePlotPanel->setStyleSheet(QString("font:12px"));

    this->addTab(realTimePanel,tr("实时"));
    this->addTab(historyPanel,tr("历史"));
    this->addTab(new QWidget(),tr("详细资料"));
    this->addTab(new QWidget(),tr("股指"));
    this->addTab(new QWidget(),tr("沪深"));
    this->addTab(new QWidget(),tr("板块"));
    this->addTab(new QWidget(),tr("港美股"));
    this->addTab(new QWidget(),tr("期货"));
    this->addTab(new QWidget(),tr("基金"));
    this->addTab(new QWidget(),tr("债券"));
    this->addTab(new QWidget(),tr("期权"));
    this->addTab(new QWidget(),tr("债券"));

    //connect(&dataFactory,SIGNAL(downloadComplete(const QString&)),this,SLOT(dataDownloaded(const QString&)));
    //dataFactory.downloadYahooDailyData(QString("600446"));
    historyPanel->installEventFilter(this);
}

void MarketPanel::setData(const QString &symbol)
{
    historyPanel->setData(HistoryDataManager::instance().getHistoryData(symbol, DataPeriod::DAY));
    realTimePanel->setData(RealTimeDataManager::instance().getRealTimeData(symbol));
    /* 互联网行情接口不提供当天分时数据。*/
    realTimePanel->setTickData(TickDataManager::instance().getTickData(symbol, "2017-09-29"));
}

void MarketPanel::loadTickData(const QVector<TickData> &data)
{

}

void MarketPanel::loadTradeData(const QVector<TradeData> &data)
{

}

/*!
 * \brief 优先将事件传递给historyPanel。
 */
void MarketPanel::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:    historyPanel->zoomIn();  return;
    case Qt::Key_Down:  historyPanel->zoomOut(); return;
    }
    QTabWidget::keyPressEvent(event);
}

