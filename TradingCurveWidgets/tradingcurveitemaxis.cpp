#include "tradingcurveitemaxis.h"
#include "abstractitemplot.h"

namespace TradingCurveWidgets{

TradingCurveItemAxis::TradingCurveItemAxis(AxisType align, AbstractItemPlot *parent):
    AbstractItemAxis(align, parent),
    itemPlot(parent)
{
}

const TradingCurveItemAxis::CandleStickSize TradingCurveItemAxis::CandleStickSizeStandard[BASIC_SCALE_LEVEL_COUNT] = {
    { 1,37,8 },
    { 1,27,7 },
    { 1,19,6 },
    { 1,13,5 },
    { 1,9,4 },
    { 1,7,4 },
    { 1,5,3 },
    { 1,3,3 },
    { 1,1,3 },
    { 1,1,1 },
    { 1,1,0}
};

int TradingCurveItemAxis::caculateItemPos(int index) const
{
    index = qBound(0, index, getItemPlot()->dataSize() - 1);

    int minVisibleIndex = getIndexRange().minIndex;
    int maxVisibleIndex = getIndexRange().maxIndex() + 1;

    //指定数据不在屏幕上显示。
    if (index<minVisibleIndex || index>maxVisibleIndex)
        return -1;

    if (currentScaleLevel < BASIC_SCALE_LEVEL_COUNT) {
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;

        //x等于该位置之前的空格数，因为坐标从0开始
        int startOffset = space/2 + (bodyWidth - 1)/2;
        return (bodyWidth + space) * (index - minVisibleIndex) + startOffset;
    }
    else {
        int overlapItemCount = currentScaleLevel - BASIC_SCALE_LEVEL_COUNT + 1;
        return (index - minVisibleIndex) / overlapItemCount;
    }

    return -1;
}

int TradingCurveItemAxis::caculateItemIndex(int x) const
{
    int itemCount;
    int minVisibleIndex = getIndexRange().minIndex;
    int maxVisibleIndex = getIndexRange().maxIndex() + 1;
    int itemIndex = getItemPlot()->selectedItemIndex(); //重新计算当前选中的数据项序号。

    if (maxVisibleIndex<0)
        return -1;
    if (getItemPlot()->dataSize() == 0)
        return -1;
    if (currentScaleLevel < BASIC_SCALE_LEVEL_COUNT) {
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;
        int startOffset = space/2 + (bodyWidth - 1)/2;

        //x等于该位置之前的空格数，因为坐标从0开始
        itemCount = qRound((double)(x - startOffset) / (bodyWidth + space));
        itemIndex = minVisibleIndex + itemCount;
        itemIndex = qBound(minVisibleIndex, itemIndex, maxVisibleIndex-1);
    }else{
        int overlapItemCount = currentScaleLevel - BASIC_SCALE_LEVEL_COUNT + 1;
        int bodyWidth = 1;

        itemCount = qRound((double)(x - 1) / bodyWidth*overlapItemCount);

        //计算时需要考虑到边界情况。
        itemIndex = qMin(maxVisibleIndex - 1, minVisibleIndex + itemCount - 1);
    }

    return itemIndex;
}

int TradingCurveItemAxis::caculatePlotPos(int index) const {
    //指定数据不在屏幕上显示。
    if (index < 0 )
        return -1;

    if (currentScaleLevel < BASIC_SCALE_LEVEL_COUNT) {
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;

        //x等于该位置之前的空格数，因为坐标从0开始
        return (bodyWidth + space) * index + space/2 + (bodyWidth - 1)/2;
    }
    else {
        int overlapItemCount = currentScaleLevel - BASIC_SCALE_LEVEL_COUNT + 1;
        return index / overlapItemCount;
    }
}

int TradingCurveItemAxis::caculatePlotIndex(int x) const
{
    int itemCount;

    int plotIndexCount = getIndexRange().size();

    int itemIndex = getItemPlot()->selectedItemIndex(); //重新计算当前选中的数据项序号。

    if (plotIndexCount < 0)
        return -1;
    if (getItemPlot()->dataSize() == 0)
        return -1;

    if (currentScaleLevel < BASIC_SCALE_LEVEL_COUNT) {
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;

        //x等于该位置之前的空格数，因为坐标从0开始
        itemCount = qRound((double) x / (bodyWidth + space));
        itemIndex = qMin(itemIndex, plotIndexCount - 1);
    }else{
        int overlapItemCount = currentScaleLevel - BASIC_SCALE_LEVEL_COUNT + 1;
        int bodyWidth = 1;

        itemCount = qRound((double)(x - 1) / bodyWidth*overlapItemCount);

        //计算时需要考虑到边界情况。
        itemIndex = qMin(plotIndexCount, itemCount - 1);
    }

    return itemIndex;
}

TradingCurveItemAxis::CandleStickSize TradingCurveItemAxis::currentItemSize() {
    if (currentScaleLevel<BASIC_SCALE_LEVEL_COUNT) {
        return CandleStickSizeStandard[currentScaleLevel];
    }
    else {
        return{ 1,1,0 };
    }
}

int TradingCurveItemAxis::bodyWidth() const{
    return currentScaleLevel < BASIC_SCALE_LEVEL_COUNT ?
                CandleStickSizeStandard[currentScaleLevel].bodyWidth : 1;
}

void TradingCurveItemAxis::onSelectedItemChanged()
{
    if(getItemPlot()->dataSize() > 0){
        int newIndex = getItemPlot()->selectedItemIndex();
        if(newIndex < getIndexRange().minIndex){
            setIndexRange(caculatePlotRangeWithMinIndex(newIndex));
        }else if(newIndex > getIndexRange().maxIndex()){
            setIndexRange(caculatePlotRangeWithMaxIndex(newIndex + 1));
        }
    }
}

void TradingCurveItemAxis::onPlotableItemAdded(){
    setIndexRange(caculatePlotRangeWithMaxIndex(getItemPlot()->dataSize() - 1));
}

void TradingCurveItemAxis::resizeEvent(QResizeEvent *event)
{
    AbstractItemAxis::resizeEvent(event);
    if (getItemPlot()->dataSize() == 0) return;
    setIndexRange(caculatePlotRangeWithMinIndex(getIndexRange().minIndex));
}

double TradingCurveItemAxis::indexPaintInterval() const
{
    if(getScaleLevel() < BASIC_SCALE_LEVEL_COUNT){
        return CandleStickSizeStandard[currentScaleLevel].bodyWidth + CandleStickSizeStandard[currentScaleLevel].space;
    }else{
        return 1.0 / (getScaleLevel() - BASIC_SCALE_LEVEL_COUNT + 1);
    }
}
void TradingCurveItemAxis::updateAxisMap()
{
    // update paint interval
    scaleDraw()->updateMap();

    // update scale interval: 和RealTimePlot的情形不同，TradingCurveItemAxis中的坐标轴上的序号位置是固定的，仅取决于
    // scaleLevel。

    QwtScaleMap& map = scaleDraw()->scaleMap();
    double lowerBound, upperBound;

    // sInterval表示对应于1个序号（index）差的位置的数值（value）差。
    double sInterval = indexPaintInterval();
    if(getScaleLevel() < BASIC_SCALE_LEVEL_COUNT){
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;
        lowerBound = getIndexRange().minIndex - ( 0.5 + (space + bodyWidth - 1) / 2) / sInterval;
    }else{
        lowerBound = getIndexRange().minIndex - 1 / sInterval;
    }

    upperBound = scaleDraw()->length() / sInterval + lowerBound;
    map.setScaleInterval(lowerBound, upperBound);

    emit axisMapChanged();
}

QwtScaleMap TradingCurveItemAxis::caculateCanvasMap(const QWidget *canvas) const
{
    QwtScaleMap map = scaleDraw()->scaleMap();

    if(scaleDraw()->orientation() == Qt::Vertical){
        map.setPaintInterval(0.0, canvas->height());
    }else{
        map.setPaintInterval(0.0, canvas->width());
    }

    qDebug() << "canvas map caculated.";
    return map;
}

void TradingCurveItemAxis::setScaleLevel(int scaleLevel)
{
    scaleLevel = qMax(0, scaleLevel);
    if(currentScaleLevel != scaleLevel){
        currentScaleLevel = scaleLevel;
        emit scaleLevelChanged();
    }
}

void TradingCurveItemAxis::updateTickLabels()
{
    IndexRange range = getIndexRange();
    int min = range.start();
    int max = range.end();

    QMap<int, QString> tickMap;

    if (getScaleLevel() < 2) {
        //每天都显示当月第几天
        for (int i = min; i < max; i++) {
            tickMap.insert(i - min, getTracerLabel(i).right(2));
        }
    }else if (getScaleLevel()<9) {
        QString month = getTracerLabel(min).mid(5, 2);
        tickMap.insert(0, month);

        //每月第一天显示月份
        for (int i = min + 1; i < max; i++) {
            QString currentMonth = getTracerLabel(i).mid(5, 2);
            if (month != currentMonth) {
                month = currentMonth;
                tickMap.insert(i - min, currentMonth);
            }
        }
    }else {
        //每年第一天显示年份
        QString year = getTracerLabel(min).left(4);
        tickMap.insert(0, year);

        //每月第一天显示月份
        for (int i = min + 1; i<max; i++) {
            QString currentYear = getTracerLabel(i).left(4);
            if (year != currentYear) {
                year = currentYear;
                tickMap.insert(i - min, currentYear);
            }
        }
    }

    setTickLabels(tickMap);
}

void TradingCurveItemAxis::updatePlotIndexRange(){
    setIndexRange(caculatePlotRangeWithMinIndex(getIndexRange().minIndex));
}

void TradingCurveItemAxis::resetIndexRange(){
    setIndexRange(caculatePlotRangeWithMinIndex(getIndexRange().minIndex));
}

void TradingCurveItemAxis::zoomIn()
{
    setScaleLevel(getScaleLevel() - 1);
    setIndexRange(caculatePlotRangeWithMidIndex(getItemPlot()->selectedItemIndex()));
    // 如果鼠标在*画布*内，则更新选中项的位置。
    if (getItemPlot()->getTracePlotCanvas()->getTracerPos().x() > 0) {
        getItemPlot()->setTracerCanvasPos(getItemPlot()->caculateTargetPos(getItemPlot()->selectedItemIndex()));
    }
    getItemPlot()->replot();
}

void TradingCurveItemAxis::zoomOut()
{
    setScaleLevel(getScaleLevel() + 1);
    setIndexRange(caculatePlotRangeWithMidIndex(getItemPlot()->selectedItemIndex()));
    // 如果鼠标在*画布*内，则更新选中项的位置。
    if (getItemPlot()->getTracePlotCanvas()->getTracerPos().x() > 0) {
        getItemPlot()->setTracerCanvasPos(getItemPlot()->caculateTargetPos(getItemPlot()->selectedItemIndex()));
    }
    getItemPlot()->replot();
}

IndexRange TradingCurveItemAxis::caculatePlotRangeWithMaxIndex(int maxIndex)
{
    int dataSize = getItemPlot()->dataSize();
    int canvasWidth = getItemPlot()->canvasWidget()->width();

    int minIndex;
    int itemCount;

    maxIndex = qMax(0, maxIndex);
    maxIndex = qMin(maxIndex, dataSize);

    if (currentScaleLevel<BASIC_SCALE_LEVEL_COUNT) {
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;
        itemCount = canvasWidth / (bodyWidth + space);
    }
    else {
        int overlapItemCount = currentScaleLevel - BASIC_SCALE_LEVEL_COUNT + 1;
        int bodyWidth = 1;
        itemCount = canvasWidth / bodyWidth*overlapItemCount;
    }

    minIndex = maxIndex - itemCount;
    //在边界情况下，尽量显示更多的数据项
    if (minIndex<0) {
        minIndex = 0;
        maxIndex = qMin(dataSize, itemCount);
    }
    return IndexRange::fromIndexInterval( minIndex, maxIndex );
}

IndexRange TradingCurveItemAxis::caculatePlotRangeWithMinIndex(int minIndex)
{
    int dataSize = getItemPlot()->dataSize();
    int canvasWidth = getItemPlot()->canvasWidget()->width();

    int maxIndex;
    int itemCount;
    minIndex = qMax(0, minIndex);
    minIndex = qMin(minIndex, dataSize);

    if (currentScaleLevel < BASIC_SCALE_LEVEL_COUNT) {
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;
        itemCount = canvasWidth / (bodyWidth + space);
    }
    else {
        int overlapItemCount = currentScaleLevel - BASIC_SCALE_LEVEL_COUNT + 1;
        int bodyWidth = 1;
        itemCount = canvasWidth / bodyWidth*overlapItemCount;
    }

    maxIndex = minIndex + itemCount;
    //在边界情况下，尽量显示更多的数据项
    if ((maxIndex > dataSize) && (itemCount < dataSize)) {
        maxIndex = dataSize;
        minIndex = qMax(0, dataSize - itemCount);
    }

    return IndexRange::fromIndexInterval( minIndex,maxIndex );
}

IndexRange TradingCurveItemAxis::caculatePlotRangeWithMidIndex(int midIndex)
{
    int dataSize = getItemPlot()->dataSize();
    int canvasWidth = getItemPlot()->canvasWidget()->width();

    int minIndex;
    int maxIndex;
    int maxItemCount;

    midIndex = qMax(0, midIndex);
    midIndex = qMin(midIndex, dataSize);

    if (currentScaleLevel<BASIC_SCALE_LEVEL_COUNT) {
        int bodyWidth = CandleStickSizeStandard[currentScaleLevel].bodyWidth;
        int space = CandleStickSizeStandard[currentScaleLevel].space;

        // 属于标准放缩范围，不出现重合
        int singleItemWidth = bodyWidth + space;
        maxItemCount = canvasWidth / singleItemWidth;
    }
    else {
        // 属于扩展放缩范围，出现柱线重合。柱线宽度始终为1，且柱线没有间距，也即是space=0
        // 重合元素数目
        int overlapItemCount = currentScaleLevel - BASIC_SCALE_LEVEL_COUNT + 1;
        int bodyWidth = 1;
        maxItemCount = canvasWidth / bodyWidth * overlapItemCount;
    }

    //确定右边所能显示数据线在数据集合中的最大序号加1（最新数据）
    minIndex = midIndex - maxItemCount / 2;
    maxIndex = minIndex + maxItemCount;

    IndexRange range = IndexRange::fromIndexInterval( minIndex, maxIndex );
    if (minIndex < 0) {
        range.moveForward(-minIndex);
    }

    return range;

    /*
    maxIndex = qMin(dataSize, midIndex + maxItemCount / 2);

    //确定左边所能显示数据项在数据集合中的最小序号（最旧数据）,如有必要，调整maxIndex。
    if (maxIndex - maxItemCount>0)
        minIndex = maxIndex - maxItemCount;
    else {
        minIndex = 0;
        maxIndex = qMin(dataSize, minIndex + maxItemCount);
    }

    return IndexRange::fromIndexInterval( minIndex, maxIndex );
    */
}

int TradingCurveItemAxis::getScaleLevel() const { return currentScaleLevel; }

const AbstractItemPlot *TradingCurveItemAxis::getItemPlot() const { return itemPlot; }

AbstractItemPlot *TradingCurveItemAxis::getItemPlot(){ return itemPlot; }

const TradingCurveItemAxis::CandleStickSize &TradingCurveItemAxis::candleStickSize() const {
    return CandleStickSizeStandard[qMin(getScaleLevel(), BASIC_SCALE_LEVEL_COUNT - 1)];
}

}
