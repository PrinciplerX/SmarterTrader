#include "realtimeplot.h"
#include "plotableitem.h"
#include <QtMath>

namespace TradingCurveWidgets {

RealTimePlot::RealTimePlot(RealTimeItemAxis::UpdateDirection direction, QWidget *parent) :
    AbstractItemPlot(parent),
    axis(new RealTimeItemAxis(AbstractAxis::BottomScale, direction, this))
{
    // this->setCanvasBackground(QBrush(Qt::black));
    // this->getTitleWidget()->setStyleSheet(QString("background: black; border-top: 1px solid #FF3030; "));

    TracePlotCanvas::TracerSetting tracerSetting = { QPen(Qt::white) };
    tracerSetting.pen.setWidth(1);
    setTracerSetting(tracerSetting);

    //坐标轴
    setScaleWidget(xBottom, axis);
    axis->setFixedBorderDist(48, 48);

    this->enableAxis(Axis::xBottom);
    this->enableAxis(Axis::yRight);
    this->enableAxis(Axis::yLeft);
    this->enableAxis(Axis::xTop, false);

    getTraceScaleWidget(yRight)->setTickLength(0, 0, 0);
    getTraceScaleWidget(xBottom)->setTickLength(0, 0, 0);
    getTraceScaleWidget(yLeft)->setTickLength(0, 0, 0);

    //getTraceScaleWidget(yRight)->setStartBorderDistPolicy(AbstractAxis::Preferred);
    //getTraceScaleWidget(yLeft)->setStartBorderDistPolicy(AbstractAxis::Preferred);

    setPlotableIndexCount(60 * 4 + 17); /* 默认为中国股市交易时段4个小时,集合竞价15分钟 */

    //设置数据绘制的数据在纵向占据全部canvas绘图范围，不留空白
    setVerticalExtraSpaceFactor(0.00);

    //设置纵轴的刻度之间的最大宽度。
    setMaxMajorTickSpace(24);

    setAxisTracerSetting({ QPen(QColor("#800000")),QBrush(QColor("#400080")),QPen(Qt::white) });

    //设置水平网格线的画笔
    QPen pen(QColor("#800000"));
    QPen dashPen(QColor("#800000"));
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 1 << space;
    dashPen.setDashPattern(dashes);

    verticalGrid.insert(15, pen);
    verticalGrid.insert(45, dashPen);
    verticalGrid.insert(75, pen);
    verticalGrid.insert(105, dashPen);
    pen.setWidth(2);
    verticalGrid.insert(135, pen);
    pen.setWidth(1);
    verticalGrid.insert(166, dashPen);
    verticalGrid.insert(196, pen);
    verticalGrid.insert(226, dashPen);

    // 时间轴刻度位置
    // 绘图序号 - 日期
    QMap<int, QString> map;
    map.insert(0, QString("09:15"));
    map.insert(15, QString("09:30"));
    map.insert(75, QString("10:30"));
    map.insert(135, QString("11:30"));
    map.insert(196, QString("14:00"));
    map.insert(256, QString("15:00"));
    axis->setTickLabels(map);
}

AbstractItemAxis *RealTimePlot::getItemAxis(){
    return axis;
}

const AbstractItemAxis *RealTimePlot::getItemAxis() const {
    return axis;
}

void RealTimePlot::drawCanvasGrid(QPainter *painter){
    //绘制水平网格线
    AbstractItemPlot::drawCanvasGrid(painter);

    //绘制竖直网格线
    if(isVerticalGridVisible()){
        for(QMap<int, QPen>::const_iterator i = verticalGrid.cbegin(); i != verticalGrid.cend(); i++){
            int x = caculateItemPos(i.key());
            painter->setPen(i.value());
            painter->drawLine(x, 0, x, getCanvas()->height() - 1);
        }
    }
}

bool RealTimePlot::isVerticalGridVisible()
{
    return verticalGridVisible;
}

void RealTimePlot::hideVerticalGrid(){
    verticalGridVisible = false;
    replot();
}

void RealTimePlot::showVerticalGrid()
{
    verticalGridVisible = true;
    replot();
}

void RealTimePlot::setVerticalGrid(const QMap<int, QPen> &verticalGrid)
{
    this->verticalGrid = verticalGrid;
}

void RealTimePlot::setPlotableIndexCount(int count)
{
    axis->setPlotableIndexCount(count);
    updateAxes();
}

int RealTimePlot::getPlotableIndexCount() const
{
    return plotableIndexCount;
}

int RealTimePlot::bodyWidth() const{
    return 1;
}

}
