#include "charttestpanel.h"
#include <QHBoxLayout>

ChartTestPanel::ChartTestPanel(QWidget *parent):
    QWidget(parent),
    chartView(new TradeChartView()),
    chart(new FChart()),
    controller(new ControlPanel(chartView, chart))
{
    //chartView->setChart(chart);
    chartView->setChart(new QChart());

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(chartView);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setFixedWidth(250);
    scrollArea->setWidget(controller);
    mainLayout->addWidget(scrollArea);
}

void ControlPanel::addCandleStickChart()
{

}

QChart *ControlPanel::createCandleStickChart()
{
    CSPlotSeries *csSeries = new CSPlotSeries();
    csSeries->setName(QString("CANDLESTICK"));
    QLineSeries* ma = new QLineSeries();
    ma->setName("MA26");
    BufferedOperation<double>* maBuf = new BufferedOperationWrapper<double>(new MA(CLOSE(mData), 26));
    maBuf->onLoaded();

    for(int i = 0; i < mData->size(); i++){
        const TradeData& data = mData->at(i);
        if(data.open >= data.close){
            CandleStickSet* item = new CandleStickSet(data.low, data.close, data.open, data.high, data.date);
            item->setBrush(Qt::green);
            item->setPen(QPen());
            csSeries->append(item);
        }else{
            CandleStickSet* item = new CandleStickSet(data.low, data.open, data.close, data.high, data.date);
            item->setBrush(Qt::red);
            item->setPen(QPen());
            csSeries->append(item);
        }
        ma->append(i, maBuf->at(i));
    }

    IndexAxis* axisX = new IndexAxis();
    axisX->setIndexLabels(TradeDataHelper::getDateSeries(mData->data()));

    QChart *chart = new QChart();
    chart->setMargins(QMargins());
    chart->addSeries(csSeries);

    chart->createDefaultAxes();
    QValueAxis *axisY = new QValueAxis;
    //axisY->setMinorTickCount(5);

    //chart->createDefaultAxes();
    //chart->axisY()->setMin(900.0);
    //chart->axisY()->setMax(6400.0);
    chart->setAxisY(axisY, csSeries);
    chart->setAxisX(axisX, csSeries);

    chart->addSeries(ma);
    ma->attachAxis(chart->axisX());
    ma->attachAxis(chart->axisY());

    chart->axisX()->setGridLineVisible(true);
    chart->axisY()->setGridLineVisible(true);
    chart->axisY()->setGridLineColor(Qt::lightGray);
    chart->axisY()->setMinorGridLineVisible(false);
    chart->axisX()->setMinorGridLineVisible(true);
    chart->axisX()->setVisible(false);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->layout()->setContentsMargins(0, 0, 0, 0);

    return chart;
}

QChart* ControlPanel::createVolumeChart()
{
    QAbstractSeries* volSeries = createVolumeSeries();

    IndexAxis* axisVolX = new IndexAxis();
    axisVolX->setIndexLabels(TradeDataHelper::getDateSeries(mData->data()));

    QChart* volChart = new QChart();
    volChart->addSeries(volSeries);
    volChart->createDefaultAxes();

    QValueAxis* axisVolY = new QValueAxis();
    axisVolY->setLabelFormat(QString::fromLocal8Bit("%.1fk"));
    volChart->setAxisY(axisVolY, volSeries);
    volChart->setAxisX(axisVolX, volSeries);
    volChart->setMargins(QMargins());

    return volChart;
}

QAbstractSeries *ControlPanel::createVolumeSeries()
{
    QBarSeries* volSeries = new QBarSeries();
    volSeries->setName(QString("VOL"));

    QBarSet* vol = new QBarSet("VOL");
    for(int i = 0; i < mData->size(); i++){
        const TradeData& data = mData->at(i);
        vol->append(data.volume/100000.0);
    }
    vol->setBrush(Qt::green);
    vol->setPen(QPen());
    volSeries->append(vol);
    return volSeries;
}
