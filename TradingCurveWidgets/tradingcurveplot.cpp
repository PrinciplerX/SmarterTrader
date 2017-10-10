#include "tradingcurveplot.h"
#include "plotableitem.h"
#include <limits>
#include <QDebug>
#include <QEvent>
#include <QApplication>
#include "tradedatahelper.h"

namespace TradingCurveWidgets {

TradingCurvePlot::TradingCurvePlot(QWidget* parent):
    AbstractItemPlot(parent),
    itemAxis(new TradingCurveItemAxis(AbstractAxis::BottomScale, this))
{
    //canvas (包括了title widget)
    //TracePlotCanvas::TracerSetting tracerSetting = { QPen(Qt::white) };
    //tracerSetting.pen.setWidth(1);
    //setTracerSetting(tracerSetting);

    //坐标轴
    QPalette p = this->palette();
    p.setColor(QPalette::WindowText, Qt::darkGray);
    itemAxis->setPalette(p);
    setScaleWidget(xBottom, itemAxis);
    itemAxis->setFixedBorderDist(0, 48);

    connect(itemAxis, SIGNAL(axisMapChanged()), this, SLOT(onAxisMapChanged()));
    connect(itemAxis, SIGNAL(indexRangeChanged(IndexRange,IndexRange)), this, SLOT(onIndexRangeChanged(IndexRange,IndexRange)));

    this->enableAxis(SimplePlot::Axis::xBottom);
    this->enableAxis(SimplePlot::Axis::yRight);
    this->enableAxis(SimplePlot::Axis::yLeft, false);
    this->enableAxis(SimplePlot::Axis::xTop, false);

    getTraceScaleWidget(yRight)->setTickLength(0, 0, 0);
    getTraceScaleWidget(xBottom)->setTickLength(0, 0, 0);

    setAxisTracerSetting({ QPen(QColor("blue")),QBrush(QColor("blue")),QPen(Qt::white) }); 
    setMaxMajorTickSpace(60);

    //内容设置
    setSelectedItemIndex(dataSize()>0 ? dataSize() - 1 : 0);
}

TradingCurvePlot::~TradingCurvePlot(){}

void TradingCurvePlot::keyPressEvent(QKeyEvent* event) {
    if (dataSize() == 0) return;
    switch (event->key()) {
    case Qt::Key_Up:
        zoomIn();
        break;
    case Qt::Key_Down:
        zoomOut();
        break;
    }
    AbstractItemPlot::keyPressEvent(event);
}

AbstractItemAxis *TradingCurvePlot::getItemAxis(){
    return itemAxis;
}

const AbstractItemAxis *TradingCurvePlot::getItemAxis() const {
    return itemAxis;
}

int TradingCurvePlot::bodyWidth() const {
    return itemAxis->bodyWidth();
}

void TradingCurvePlot::setHistoryData(AbstractHistoryData *data)
{
    if(this->data)
        disconnect(data, SIGNAL(dataLoaded()), this, SLOT(onDataLoaded()));

    this->data = data;
    connect(data, SIGNAL(dataLoaded()), this, SLOT(onDataLoaded()));
}

void TradingCurvePlot::zoomIn()
{
    itemAxis->zoomIn();
}

void TradingCurvePlot::zoomOut()
{
    itemAxis->zoomOut();
}

void TradingCurvePlot::onDataUpdated()
{

}

void TradingCurvePlot::onDataLoaded()
{
    itemAxis->setTracerLabels(TradeDataHelper::getDateSeries(data->data()));
    getItemAxis()->updatePlotIndexRange();
    updateLayout();
    updateHorizontialAxes();
    updateVerticalAxes();
    replot();
}

}
