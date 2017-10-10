#include "historypanel.h"
#include <QVBoxLayout>

#include "boll.h"
#include "kdj.h"
#include "dmi.h"
#include "volume.h"
#include "macd.h"
#include "tradedatahelper.h"
#include "historydataparser.h"
#include "yahooquotefactory.h"
#include "rsi.h"
#include "bar.h"
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include "plotablecurvefactory.h"
#include "operationwrapperindicator.h"

HistoryPanel::HistoryPanel(int plotCount, QWidget *parent):
    DetailedPanel(DetailedPanel::Right, nullptr, parent),
    panel(new TradingCurvePlotPanel()),
    indicatorTabBar(new QTabBar()),
    topPlot(new TradingCurvePlot()),
    midPlot(new TradingCurvePlot()),
    btmPlot(new TradingCurvePlot())
{
    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    setMainPanel(mainWidget);

    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(panel);
    mainLayout->addWidget(indicatorTabBar);

    indicatorTabBar->setObjectName(QString("ChoiceTabBar"));
    indicatorTabBar->addTab(QString("MACD"));
    indicatorTabBar->addTab(QString("KDJ"));
    indicatorTabBar->addTab(QString("EMA"));
    indicatorTabBar->addTab(QString("BOLL"));
    //indicatorTabBar->addTab(QString("主力"));
    indicatorTabBar->addTab(QString("W&&R"));
    indicatorTabBar->addTab(QString("DMI"));
    indicatorTabBar->addTab(QString("BIAS"));
    indicatorTabBar->addTab(QString("ASI"));
    indicatorTabBar->addTab(QString("VR"));
    indicatorTabBar->addTab(QString("ARBR"));
    indicatorTabBar->addTab(QString("DPO"));
    indicatorTabBar->addTab(QString("TRIX"));
    indicatorTabBar->addTab(QString("新DMA"));

    indicatorTabBar->setShape(QTabBar::TriangularSouth);
    indicatorTabBar->setExpanding(false);

    setPlotCount(plotCount);

    QPushButton* button = new QPushButton(QString("->"));
    initButton(button);
    connect(button, SIGNAL(clicked()), this, SLOT(onSidePanelButtonClicked()));

    QPushButton* favoriteButton = new QPushButton("加自选");
    QPushButton* plotItemButton = new QPushButton("指标");
    initButton(favoriteButton);
    initButton(plotItemButton);

    //topPlot->addTitleWidget(plotItemButton);
    //topPlot->addTitleWidget(favoriteButton);
    //topPlot->addTitleWidget(button);

    panel->setFrameShape(StyledPanel);

    panel->addTradingCurvePlot(topPlot);
    panel->addTradingCurvePlot(midPlot);
    panel->addTradingCurvePlot(btmPlot);
    panel->show();

    this->sidePanel()->setMinimumWidth(210);
    this->sidePanel()->setTabPosition(QTabWidget::South);
    this->sidePanel()->setTabShape(QTabWidget::Triangular);
    this->sidePanel()->tabBar()->setObjectName(QString("ChoiceTabBar"));
    this->sidePanel()->addTab(new QWidget(), QString("控制"));
    this->sidePanel()->addTab(new QWidget(), QString("详细"));
    this->sidePanel()->addTab(new QWidget(), QString("相关"));
    this->sidePanel()->addTab(new QWidget(), QString("扩展"));

    connect(indicatorTabBar, SIGNAL(tabBarClicked(int)), this, SLOT(onIndicatorBarClicked(int)));
    hideSidePanel();
}

void HistoryPanel::addIndicator(int plotIndex, PlotableItem *item)
{
    panel->getPlotAt(plotIndex)->addPlotableItem(item);
}

void HistoryPanel::initButton(QPushButton *button)
{
    QString borderStyleSheet("border: 0px; border-left: 1px solid #b9b9b9; font: 12px;");
    QString indexLabelStyleSheet = QString("QPushButton{background-color:#f9f9f9;") + borderStyleSheet + QString("}") +
                                   QString("QPushButton:hover{background-color:#fbfbfb;") + borderStyleSheet + QString("}") +
                                   QString("QPushButton:pressed{background-color:#dddddd;") + borderStyleSheet + QString("}");
    button->setStyleSheet(indexLabelStyleSheet);
}

void HistoryPanel::zoomIn()
{
    panel->zoomIn();
}

void HistoryPanel::zoomOut()
{
    panel->zoomOut();
}

void HistoryPanel::setData(AbstractHistoryData *data)
{
    if(mData != nullptr){
        disconnect(mData, SIGNAL(dataAppended()), this, SLOT(onDataAppended()));
        disconnect(mData, SIGNAL(dataLoaded()), this, SLOT(onDataLoaded()));
        disconnect(mData, SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));
    }

    mData = data;

    connect(mData, SIGNAL(dataAppended()), this, SLOT(onDataAppended()));
    connect(mData, SIGNAL(dataLoaded()), this, SLOT(onDataLoaded()));
    connect(mData, SIGNAL(dataUpdated()), this, SLOT(onDataUpdated()));

    panel->setData(mData);
    panel->getPlotAt(0)->clearPlotableItem();
    panel->getPlotAt(1)->clearPlotableItem();
    panel->getPlotAt(2)->clearPlotableItem();

    /*--------------------------------均线系统--------------------------------*/
    CandleStick* cs = new CandleStick(mData);
    PlotableCurve* ma5 = PlotableCurveFactory::createMA(mData, 5);//, QPen(Qt::darkBlue));
    PlotableCurve* ma10 = PlotableCurveFactory::createMA(mData, 10);//, QPen(Qt::darkBlue));
    PlotableCurve* ma20 = PlotableCurveFactory::createMA(mData, 20);//, QPen(Qt::darkBlue));
    /*------------------------------------------------------------------------*/

    addIndicator(0, cs);
    addIndicator(0, ma5);
    addIndicator(0, ma10);
    addIndicator(0, ma20);

    mData->attachIndicator(cs);
    mData->attachIndicator(new OperationWrapperIndicator(ma5->operation(), QString("MA"), QString("MA5")));
    mData->attachIndicator(new OperationWrapperIndicator(ma10->operation(), QString("MA"), QString("MA10")));
    mData->attachIndicator(new OperationWrapperIndicator(ma20->operation(), QString("MA"), QString("MA20")));

    Volume* volume = new Volume(mData);
    PlotableCurve* mavol5 = PlotableCurveFactory::createMAVOL(mData, 5);//, QPen(Qt::darkGray));
    PlotableCurve* mavol10 = PlotableCurveFactory::createMAVOL(mData, 10);//new MAVOL(mData->data(), 10, QPen(Qt::darkBlue));

    addIndicator(1, volume);
    addIndicator(1, mavol5);
    addIndicator(1, mavol10);

    mData->attachIndicator(volume);
    mData->attachIndicator(new OperationWrapperIndicator(mavol5->operation(), QString("MAVOL"), QString("MAVOL5")));
    mData->attachIndicator(new OperationWrapperIndicator(mavol10->operation(), QString("MAVOL"), QString("MAVOL10")));

    PlotableCurve* obv = PlotableCurveFactory::createOBV(mData);
    mData->attachIndicator(new OperationWrapperIndicator(obv->operation(), QString("OBV"), QString("OBV")));
    addIndicator(2, obv);
}

void HistoryPanel::setPlotCount(int count)
{
}

void HistoryPanel::onDataAppended()
{

}

void HistoryPanel::onDataUpdated()
{

}

void HistoryPanel::onDataLoaded()
{
    TradeDataHelper::saveToCsv(mData->getSymbol() + ".csv", mData->data());
}

void HistoryPanel::onIndicatorBarClicked(int index)
{
    static TechniqueIndicator* currentIndicator = nullptr;
    static PlotableItem* plotableItem = nullptr;

    TechniqueIndicator* oldIndicator = currentIndicator;

    /*
    switch (index) {
    case 0: {
        MACD* i = new MACD();
        plotableItem = i;
        currentIndicator = i;
        break;
    }
    case 1: {
        KDJ* i = new KDJ();
        plotableItem = i;
        currentIndicator = i;
        break;
    }
    case 2: {
        EMA* i = new EMA();
        plotableItem = i;
        currentIndicator = i;
        break;
    }
    case 3: {
        BOLL* i = new BOLL();
        plotableItem = i;
        currentIndicator = i;
        break;
    }
    case 4: {
        WMS* i = new WMS();
        plotableItem = i;
        currentIndicator = i;
        break;
    }
    case 5: {
        DMI* i = new DMI();
        plotableItem = i;
        currentIndicator = i;
        break;
    }
    case 6: {
        BIAS* i = new BIAS();
        plotableItem = i;
        currentIndicator = i;
        break;
    }
    default:
        QMessageBox::information(this, QString("提示"), QString("所选指标尚未实现！"));
        return;
    }

    // 如果已经链接到相同的指标，则应继续用该指标而不是替换它。
    if(!mData->attachIndicator(currentIndicator)){
        TechniqueIndicator* indicator = mData->findIndicator(currentIndicator->indicatorName());
        qDebug() << btmPlot->containsItem((PlotableItem*)indicator);
        return;
    }

    btmPlot->clearPlotableItem();
    mData->removeIndicator(oldIndicator);
    btmPlot->addPlotableItem(plotableItem);
    btmPlot->replot();
    */
}

void HistoryPanel::onSidePanelButtonClicked()
{
    if(sidePanel()->isVisible())
       sidePanel()->hide();
    else
       sidePanel()->show();
}

