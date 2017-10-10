#include "realtimeitemaxis.h"
#include "abstractitemplot.h"

namespace TradingCurveWidgets{

RealTimeItemAxis::RealTimeItemAxis(AxisType align, UpdateDirection direction, QWidget *parent):
    AbstractItemAxis(align, parent),
    updateDirection(direction)
{

}

void RealTimeItemAxis::setPlotableIndexCount(int count){
    plotableIndexCount = count;
    updateLayout();
}

int RealTimeItemAxis::getPlotableIndexCount() const {
    return plotableIndexCount;
}

QwtScaleMap RealTimeItemAxis::caculateCanvasMap(const QWidget *canvas) const
{
    QwtScaleMap map = scaleDraw()->scaleMap();

    if(scaleDraw()->orientation() == Qt::Vertical){
        map.setPaintInterval(0.0, canvas->height());
    }else{
        map.setPaintInterval(0.0, canvas->width());
    }

    return map;
}

void RealTimeItemAxis::updateAxisMap()
{
    scaleDraw()->updateMap();
    QwtScaleMap& map = scaleDraw()->scaleMap();

    // sInterval表示对应于1个序号（index）差的位置的数值（value）差。
    double sInterval = (scaleDraw()->length() - 1.0) / (plotableIndexCount - 1);
    // canvas两端表示的数值。
    double lowerBound = 0 /* 或（0.5 * sInterval）*/ - 0.5 / sInterval;
    double upperBound = scaleDraw()->length() / sInterval + lowerBound;
    map.setScaleInterval(lowerBound, upperBound);
}

/*
int RealTimeItemAxis::caculateItemPos(int index) const
{
    int dataSize = getIndexRange().offset;
    int canvasWidth = getItemPlot()->canvasWidget()->width();

    if(dataSize <= index) return -1;

    // 起点和终点贴紧canvas界，所以canvas水平方向被分为 getPlotableIndexCount() - 1 个interval。
    int intervalCount = getPlotableIndexCount() - 1;
    double intervalLength = canvasWidth / (double)intervalCount;

    if(dataSize >= getPlotableIndexCount()){
        switch(updateDirection){
        case OldestLeft:
        case NewestRight:
            return qBound(0, qRound(index * intervalLength), canvasWidth - 1);
        case OldestRight:
        case NewestLeft:
            return qBound(0, qRound(canvasWidth - index * intervalLength), canvasWidth - 1);
        }
    }else{
        int totalLength = dataSize * intervalLength;
        switch(updateDirection){
        case OldestLeft:  return qBound(0, qRound(index * intervalLength), canvasWidth - 1);
        case OldestRight: return qBound(0, qRound(canvasWidth - index * intervalLength), canvasWidth - 1);
        case NewestLeft:  return qBound(0, totalLength - qRound(index * intervalLength), canvasWidth - 1);
        case NewestRight: return qBound(0, qRound(canvasWidth - (dataSize - 1 - index) * intervalLength), canvasWidth - 1);
        }
    }

    return -1;
}

int RealTimeItemAxis::caculateItemIndex(int x) const
{
    int dataSize = getItemPlot()->dataSize();
    int canvasWidth = getItemPlot()->canvasWidget()->width();

    // 起点和终点贴紧canvas界，所以canvas水平方向被分为 getPlotableIndexCount() - 1 个interval。
    // 取四舍五入选择最靠近的Index
    int intervalCount = getPlotableIndexCount() - 1;
    double intervalLength = canvasWidth / (double)intervalCount;

    if(dataSize >= getPlotableIndexCount()){
        switch(updateDirection){
        case OldestLeft:
        case NewestRight:
            return qBound(0, qRound(x / intervalLength), dataSize - 1);
        case OldestRight:
        case NewestLeft:
            return qBound(0, qRound((canvasWidth - x) / intervalLength), dataSize - 1);
        }
    }else{
        switch(updateDirection){
        case OldestLeft:  return qBound(0, qRound(x / intervalLength), dataSize - 1);
        case OldestRight: return qBound(0, qRound((canvasWidth - x) / intervalLength), dataSize - 1);
        case NewestLeft:  return qBound(0, dataSize - 1 - qRound(x / intervalLength), dataSize - 1);
        case NewestRight: return qBound(0, dataSize - 1 - qRound((canvasWidth - x) / intervalLength), dataSize - 1);
        }
    }

    return -1;
}

int RealTimeItemAxis::caculatePlotIndex(int x) const
{
    int canvasWidth = getItemPlot()->canvasWidget()->width();
    // 起点和终点贴紧canvas边界，所以canvas水平方向被分为 getPlotableIndexCount() - 1 个interval。
    // 取四舍五入选择最靠近的Index
    int intervalCount = getPlotableIndexCount() - 1;
    double intervalLength = canvasWidth / (double)intervalCount;
    return qBound(0, qRound(x / intervalLength), intervalCount + 1);
}

int RealTimeItemAxis::caculatePlotPos(int index) const
{
    int canvasWidth = getItemPlot()->canvasWidget()->width();
    int intervalCount = getPlotableIndexCount() - 1;
    double intervalLength = canvasWidth / (double)intervalCount;
    return qBound(0, qRound(index * intervalLength), canvasWidth - 1);
}
*/

void RealTimeItemAxis::onSelectedItemChanged() {}

void RealTimeItemAxis::onPlotableItemAdded() {}

void RealTimeItemAxis::updatePlotIndexRange() {}

int RealTimeItemAxis::bodyWidth() const {
    return 1;
}

void RealTimeItemAxis::updateTickLabels() {}

}
