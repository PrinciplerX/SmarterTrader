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
    tabBar->addTab(QString("����"));
    tabBar->addTab(QString("ָ��ƽ̨"));
    tabBar->addTab(QString("��Ѷ"));
    tabBar->addTab(QString("MACD"));
    tabBar->addTab(QString("KDJ"));
    tabBar->addTab(QString("RSI"));
    tabBar->addTab(QString("BOLL"));
    tabBar->addTab(QString("����"));
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

    this->addTab(realTimePanel,tr("ʵʱ"));
    this->addTab(historyPanel,tr("��ʷ"));
    this->addTab(new QWidget(),tr("��ϸ����"));
    this->addTab(new QWidget(),tr("��ָ"));
    this->addTab(new QWidget(),tr("����"));
    this->addTab(new QWidget(),tr("���"));
    this->addTab(new QWidget(),tr("������"));
    this->addTab(new QWidget(),tr("�ڻ�"));
    this->addTab(new QWidget(),tr("����"));
    this->addTab(new QWidget(),tr("ծȯ"));
    this->addTab(new QWidget(),tr("��Ȩ"));
    this->addTab(new QWidget(),tr("ծȯ"));

    //connect(&dataFactory,SIGNAL(downloadComplete(const QString&)),this,SLOT(dataDownloaded(const QString&)));
    //dataFactory.downloadYahooDailyData(QString("600446"));
    historyPanel->installEventFilter(this);
}

void MarketPanel::setData(const QString &symbol)
{
    historyPanel->setData(HistoryDataManager::instance().getHistoryData(symbol, DataPeriod::DAY));
    realTimePanel->setData(RealTimeDataManager::instance().getRealTimeData(symbol));
    /* ����������ӿڲ��ṩ�����ʱ���ݡ�*/
    realTimePanel->setTickData(TickDataManager::instance().getTickData(symbol, "2017-09-29"));
}

void MarketPanel::loadTickData(const QVector<TickData> &data)
{

}

void MarketPanel::loadTradeData(const QVector<TradeData> &data)
{

}

/*!
 * \brief ���Ƚ��¼����ݸ�historyPanel��
 */
void MarketPanel::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:    historyPanel->zoomIn();  return;
    case Qt::Key_Down:  historyPanel->zoomOut(); return;
    }
    QTabWidget::keyPressEvent(event);
}

