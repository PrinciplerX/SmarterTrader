#include "realtimecompositeplot.h"
#include "percentagescaledraw.h"
#include "realtimecurvescaledraw.h"
#include "integeraxisdraw.h"
#include "voidaxisdraw.h"

namespace TradingCurveWidgets {

RealTimeCompositePlot::RealTimeCompositePlot(double baseValue, QWidget *parent):
    SplitterPlot(Qt::Vertical, 2, parent),
    percentageScaleDraw(new PercentageScaleDraw(baseValue)),
    realTimeCurveScaleDraw(new RealTimeCurveScaleDraw(baseValue))
{
    SplitterScaleWidget* leftAxis = (SplitterScaleWidget*)getAxisWidget(yLeft);
    SplitterScaleWidget* rightAxis = (SplitterScaleWidget*)getAxisWidget(yRight);

    initPricePlotScaleDraw(percentageScaleDraw);
    initPricePlotScaleDraw(realTimeCurveScaleDraw);
    IntegerAxisDraw* vLeftAxisDraw = new IntegerAxisDraw();
    VoidAxisDraw* vRightAxisDraw = new VoidAxisDraw();

    initPricePlotScaleDraw(vLeftAxisDraw);
    initPricePlotScaleDraw(vRightAxisDraw);

    rightAxis->setScaleDraw(percentageScaleDraw, 0);
    leftAxis->setScaleDraw(realTimeCurveScaleDraw, 0);
    rightAxis->setScaleDraw(vRightAxisDraw, 1);
    leftAxis->setScaleDraw(vLeftAxisDraw, 1);

    initPlotColor();
    getTitleWidget()->setStyleSheet(QString("color: white;"));

    // 竖直网格线
    // 设置水平网格线的画笔
    QPen pen(QColor("#800000"));
    QPen dashPen(QColor("#800000"));
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 1 << space;
    dashPen.setDashPattern(dashes);

    mVerticalGrid.insert(15, pen);
    mVerticalGrid.insert(45, dashPen);
    mVerticalGrid.insert(75, pen);
    mVerticalGrid.insert(105, dashPen);
    pen.setWidth(2);
    mVerticalGrid.insert(135, pen);
    pen.setWidth(1);
    mVerticalGrid.insert(166, dashPen);
    mVerticalGrid.insert(196, pen);
    mVerticalGrid.insert(226, dashPen);
}

void RealTimeCompositePlot::setBaseValue(double value)
{
    percentageScaleDraw->setBaseValue(value);
    realTimeCurveScaleDraw->setBaseValue(value);
}

double RealTimeCompositePlot::baseValue() const
{
    return percentageScaleDraw->baseValue();
}

void RealTimeCompositePlot::setData(AbstractHistoryData *data)
{
    if(mData){
        disconnect(mData, SIGNAL(dataLoaded()), this, SLOT(onDataChanged()));
        disconnect(mData, SIGNAL(dataAppended()), this, SLOT(onDataChanged()));
        disconnect(mData, SIGNAL(dataUpdated()), this, SLOT(onDataChanged()));
    }

    mData = data;
    if(!mData) return;

    connect(mData, SIGNAL(dataLoaded()), this, SLOT(onDataChanged()));
    connect(mData, SIGNAL(dataAppended()), this, SLOT(onDataChanged()));
    connect(mData, SIGNAL(dataUpdated()), this, SLOT(onDataChanged()));
}

void RealTimeCompositePlot::drawCanvasGrid(QWidget *canvas, QPainter *painter)
{
    SplitterPlot::drawCanvasGrid(canvas, painter);
    drawVerticalGrid(canvas, painter);
}

void RealTimeCompositePlot::drawVerticalGrid(QWidget *canvas, QPainter *painter)
{
    if(isVerticalGridVisible()){
        QwtScaleMap map = itemAxis()->caculateCanvasMap(canvas);
        for(QMap<int, QPen>::const_iterator i = mVerticalGrid.cbegin();
            i != mVerticalGrid.cend(); i++){
            int x = map.transform(i.key());
            painter->setPen(i.value());
            painter->drawLine(x, 0, x, canvas->height() - 1);
        }
    }
}

void RealTimeCompositePlot::updateAxis(int canvasIndex)
{
    if(plotableItemCount(canvasIndex) == 0) return;
    if(dataSize(canvasIndex) == 0) return;

    IndexRange range = itemAxis()->getIndexRange();
    double maxValue = regionalMaxValue(range.minIndex, range.maxIndex() + 1, canvasIndex);
    double minValue = regionalMinValue(range.minIndex, range.maxIndex() + 1, canvasIndex);

    if(orientation() == Qt::Vertical){
        if(canvasIndex == 0){
            double base = baseValue();
            double interval = qMax(maxValue - base, base - minValue);
            minValue = base - interval;
            maxValue = base + interval;
        }
        ((SplitterScaleWidget*)getTraceAxis(yRight))->setValueRange(minValue, maxValue, canvasIndex);
        ((SplitterScaleWidget*)getTraceAxis(yLeft))->setValueRange(minValue, maxValue, canvasIndex);
    }else{
        ((SplitterScaleWidget*)getTraceAxis(xTop))->setValueRange(minValue, maxValue, canvasIndex);
        ((SplitterScaleWidget*)getTraceAxis(xBottom))->setValueRange(minValue, maxValue, canvasIndex);
    }
}

bool RealTimeCompositePlot::isVerticalGridVisible() const
{
    return mVerticalGridVisible;
}

void RealTimeCompositePlot::setVerticalGridVisible(bool visible)
{
    mVerticalGridVisible = visible;
    replot();
}

void RealTimeCompositePlot::initPricePlotScaleDraw(RectangularAxisDraw *scaleDraw)
{
    scaleDraw->setTickLength(QwtScaleDiv::MajorTick, 0);
    scaleDraw->setTickLength(QwtScaleDiv::MediumTick, 0);
    scaleDraw->setTickLength(QwtScaleDiv::MinorTick, 0);
}

void RealTimeCompositePlot::initPlotColor()
{
    // #02E2F4 - 淡蓝色 - volume plot axis label
    // #800000 - 暗红色 - backbone, grid line
    // #FF3232 - 鲜红色 - price plot axis label

    // QPalette::Text - 刻度脚标
    // QPalette::WindowText - 坐标轴线
    // QPalette::HighlightedText - 游标文字

    SplitterScaleWidget* leftAxis = (SplitterScaleWidget*)getAxisWidget(yLeft);
    SplitterScaleWidget* rightAxis = (SplitterScaleWidget*)getAxisWidget(yRight);

    QPalette axisPalette = palette();
    axisPalette.setColor(QPalette::Window, Qt::black);
    axisPalette.setColor(QPalette::WindowText, QColor("#800000"));

    leftAxis->setAxisPalette(axisPalette, 0);
    rightAxis->setAxisPalette(axisPalette, 0);

    axisPalette.setColor(QPalette::Text, QColor("#FF3232"));
    getAxisWidget(xBottom)->setPalette(axisPalette);

    axisPalette.setColor(QPalette::Text, QColor("#02E2F4"));
    axisPalette.setColor(QPalette::HighlightedText, QColor("#02E2F4"));
    leftAxis->setAxisPalette(axisPalette, 1);
    rightAxis->setAxisPalette(axisPalette, 1);
}

void RealTimeCompositePlot::onDataChanged()
{
    setBaseValue(mData->at(0).open);
    updateAxes();
    replot();
}

}
