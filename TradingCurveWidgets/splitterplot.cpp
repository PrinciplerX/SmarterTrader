#include "splitterplot.h"
#include "tracescalewidget.h"
#include "abstractplotlayout.h"
#include <cfloat>

namespace TradingCurveWidgets{

SplitterPlot::SplitterPlot(Qt::Orientation orientation, int canvasCount, QWidget *parent):
    AbstractTracePlot(parent),
    splitterCanvas(new SplitterPlotCanvas(orientation, canvasCount, this)),
    plotLayout(new AbstractPlotLayout()),
    titleWidget(new PlotTitleWidget("没有数据")),
    mItemAxis(new RealTimeItemAxis(AbstractAxis::BottomScale, RealTimeItemAxis::OldestLeft, this))
{
    titleWidget->setFixedHeight(15);
    splitterCanvas->insertWidget(0, titleWidget);
    setCanvas(splitterCanvas);
    splitterCanvas->setCanvasBackground(Qt::black);

    connect(splitterCanvas, SIGNAL(splitterMoved(int,int)), this, SLOT(onCanvasResized()));
    connect(splitterCanvas, SIGNAL(canvasResized()), this, SLOT(onCanvasResized()));
    connect(splitterCanvas, SIGNAL(targetMoved(QPoint)), this, SLOT(onTargetMove(QPoint)));
    connect(splitterCanvas, SIGNAL(targetLeaved()), this, SLOT(onTargetLeave()));

    if(orientation == Qt::Vertical){
        SplitterScaleWidget* sa1 = new SplitterScaleWidget(AbstractAxis::LeftScale, canvasCount, this);
        SplitterScaleWidget* sa2 = new SplitterScaleWidget(AbstractAxis::RightScale, canvasCount, this);
        TraceScaleWidget* ta2 = new TraceScaleWidget(AbstractAxis::TopScale, this);

        // 时间轴刻度位置
        // 绘图序号 - 日期
        QMap<int, QString> map;
        map.insert(0, QString("09:15"));
        map.insert(15, QString("09:30"));
        map.insert(75, QString("10:30"));
        map.insert(135, QString("11:30"));
        map.insert(196, QString("14:00"));
        map.insert(256, QString("15:00"));
        mItemAxis->setTickLabels(map);
        mItemAxis->setIndexRange(0, 257);

        mItemAxis->enableAxis();
        sa1->setTickLength(0, 0, 0);
        sa2->setTickLength(0, 0, 0);
        mItemAxis->setTickLength(0, 0, 0);
        mItemAxis->setFixedBorderDist(48, 48);
        ta2->setTickLength(0, 0, 0);
        axisArray[xBottom] = mItemAxis;
        axisArray[xTop] = ta2;
        axisArray[yLeft] = sa1;
        axisArray[yRight] = sa2;

        connect(splitterCanvas, SIGNAL(canvasResized(int)), sa1, SLOT(onCanvasResized(int)));
        connect(splitterCanvas, SIGNAL(canvasResized(int)), sa2, SLOT(onCanvasResized(int)));

    }else{
        SplitterScaleWidget* sa1 = new SplitterScaleWidget(AbstractAxis::BottomScale, canvasCount, this);
        SplitterScaleWidget* sa2 = new SplitterScaleWidget(AbstractAxis::TopScale, canvasCount, this);
        TraceScaleWidget* ta1 = new TraceScaleWidget(AbstractAxis::LeftScale, this);
        TraceScaleWidget* ta2 = new TraceScaleWidget(AbstractAxis::RightScale, this);

        sa1->setTickLength(0, 0, 0);
        sa2->setTickLength(0, 0, 0);
        ta1->setTickLength(0, 0, 0);
        ta2->setTickLength(0, 0, 0);
        axisArray[xBottom] = sa1;
        axisArray[xTop] = sa2;
        axisArray[yLeft] = ta1;
        axisArray[yRight] = ta2;

        connect(splitterCanvas, SIGNAL(canvasResized(int)), sa1, SLOT(onCanvasResized(int)));
        connect(splitterCanvas, SIGNAL(canvasResized(int)), sa2, SLOT(onCanvasResized(int)));
    }

    axisArray[xTop]->enableAxis(false);
    axisArray[xTop]->enableTracking(false);

    // 设置配色
    QPalette axisPalette = this->palette();
    axisPalette.setColor(QPalette::Window, Qt::black);
    axisPalette.setColor(QPalette::WindowText, QColor("#800000"));

    setStyleSheet(QString("background: black; font-size: 12px;"));
    getAxisWidget(AbstractPlot::xTop)->setPalette(axisPalette);
    getAxisWidget(AbstractPlot::yLeft)->setPalette(axisPalette);
    getAxisWidget(AbstractPlot::yRight)->setPalette(axisPalette);
}

Qt::Orientation SplitterPlot::orientation() const {
    return splitterCanvas->orientation();
}

void SplitterPlot::setTraceAxis(int axisId, AbstractTraceAxis *traceAxis)
{
    if(axisArray[axisId])
        delete axisArray[axisId];
    traceAxis->setParent(this);
    axisArray[axisId] = traceAxis;
}

void SplitterPlot::setSplitterAxis(int axisId, SplitterScaleWidget *splitterAxis)
{
    if(isSplitterAxis(axisId)){
        if(axisArray[axisId] != nullptr){
            if(splitterCanvas != nullptr){
                disconnect(splitterCanvas, SIGNAL(canvasResized(int)), axisArray[axisId], SLOT(onCanvasResized(int)));
                connect(splitterCanvas, SIGNAL(canvasResized(int)), splitterAxis, SLOT(onCanvasResized(int)));
            }
            delete axisArray[axisId];
        }
        splitterAxis->setParent(this);
        axisArray[axisId] = splitterAxis;
    }
}

bool SplitterPlot::isSplitterAxis(int axisId)
{
    if(orientation() == Qt::Vertical){
        return (axisId == yLeft) || (axisId == yRight);
    }
    return (axisId == xTop) || (axisId == xBottom);
}

void SplitterPlot::setTracerLabels(QVector<QString> &labels)
{
    mItemAxis->setTracerLabels(labels);
}

QwtScaleMap SplitterPlot::canvasMap(TracePlotCanvas *canvas, int axisId) const
{
    return QwtScaleMap();
}

QwtScaleMap SplitterPlot::canvasMap(int canvasIndex, int axisId) const
{
    return ((SplitterScaleWidget*)(axisArray[yRight]))->caculateCanvasMap(canvasIndex);
}

void SplitterPlot::updateAxes()
{
    for(int i = 0; i < itemVector.size(); i++){
        updateAxis(i);
        updateAxesDiv(i);
    }
    mItemAxis->updateAxisMap();
    mItemAxis->updateAxisDiv();
}

void SplitterPlot::updateAxis(int canvasIndex)
{
    if(itemVector[canvasIndex].size() == 0) return;
    if(itemVector[canvasIndex].at(0)->size() == 0) return;

    IndexRange range = mItemAxis->getIndexRange();
    double maxValue = regionalMaxValue(range.minIndex, range.maxIndex() + 1, canvasIndex);
    double minValue = regionalMinValue(range.minIndex, range.maxIndex() + 1, canvasIndex);

    if(orientation() == Qt::Vertical){
        ((SplitterScaleWidget*)axisArray[yRight])->setValueRange(minValue, maxValue, canvasIndex);
        ((SplitterScaleWidget*)axisArray[yLeft])->setValueRange(minValue, maxValue, canvasIndex);
    }else{
        ((SplitterScaleWidget*)axisArray[xTop])->setValueRange(minValue, maxValue, canvasIndex);
        ((SplitterScaleWidget*)axisArray[xBottom])->setValueRange(minValue, maxValue, canvasIndex);
    }
}

void SplitterPlot::updateCanvasRange()
{
    for(int i = 0; i < getCanvasCount(); i++){
        updateCanvasRange(i);
    }
}

void SplitterPlot::updateCanvasRange(int canvasIndex)
{
    if(itemVector[canvasIndex].size() == 0) return;

    IndexRange range = mItemAxis->getIndexRange();
    double maxValue = regionalMaxValue(range.minIndex, range.maxIndex() + 1, canvasIndex);
    double minValue = regionalMinValue(range.minIndex, range.maxIndex() + 1, canvasIndex);

    if(orientation() == Qt::Vertical){
        ((SplitterScaleWidget*)axisArray[yRight])->setValueRange(minValue, maxValue, canvasIndex);
        ((SplitterScaleWidget*)axisArray[yLeft])->setValueRange(minValue, maxValue, canvasIndex);
    }else{
        ((SplitterScaleWidget*)axisArray[xTop])->setValueRange(minValue, maxValue, canvasIndex);
        ((SplitterScaleWidget*)axisArray[xBottom])->setValueRange(minValue, maxValue, canvasIndex);
    }
}

void SplitterPlot::updateAxesDiv(int canvasIndex)
{
    if(orientation() == Qt::Vertical){
        ((SplitterScaleWidget*)axisArray[yLeft])->updateScaleDiv(canvasIndex);
        ((SplitterScaleWidget*)axisArray[yRight])->updateScaleDiv(canvasIndex);
    }else{
        ((SplitterScaleWidget*)axisArray[xTop])->updateScaleDiv(canvasIndex);
        ((SplitterScaleWidget*)axisArray[xBottom])->updateScaleDiv(canvasIndex);
    }
}

void SplitterPlot::updateCanvasMap()
{
}

void SplitterPlot::createDefaultAxis(int axisId)
{
    if(splitterCanvas->orientation() == Qt::Vertical){
        switch(axisId){
        case xTop: axisArray[xTop] = new TraceScaleWidget(AbstractAxis::TopScale, this); break;
        case xBottom: axisArray[xBottom] = new TraceScaleWidget(AbstractAxis::BottomScale, this); break;
        case yRight: axisArray[yRight] = new SplitterScaleWidget(AbstractAxis::RightScale, splitterCanvas->count(), this); break;
        case yLeft: axisArray[yLeft] = new SplitterScaleWidget(AbstractAxis::LeftScale, splitterCanvas->count(), this); break;
        }
    }else{
        switch(axisId){
        case xTop: axisArray[xTop] = new SplitterScaleWidget(AbstractAxis::TopScale, splitterCanvas->count(), this); break;
        case xBottom: axisArray[xBottom] = new SplitterScaleWidget(AbstractAxis::BottomScale, splitterCanvas->count(), this); break;
        case yRight: axisArray[yRight] = new TraceScaleWidget(AbstractAxis::RightScale, this); break;
        case yLeft: axisArray[yLeft] = new TraceScaleWidget(AbstractAxis::LeftScale, this); break;
        }
    }
}

void SplitterPlot::addPlotableItem(PlotableItem *item, int canvasIndex)
{
    if(itemVector.size() <= canvasIndex + 1){
        itemVector.resize(canvasIndex + 1);
        itemVector[canvasIndex].append(item);
    }
    updateAxes();
    replot();
}

int SplitterPlot::plotableItemCount(int canvasIndex) const
{
    return itemVector[canvasIndex].size();
}

void SplitterPlot::clearPlotableItem()
{
    itemVector.clear();
}

void SplitterPlot::clearPlotableItem(int canvasIndex)
{
    itemVector[canvasIndex].clear();
    updateAxes();
    replot();
}

double SplitterPlot::regionalMaxValue(int minIndex, int maxIndex, int canvasId) const {
    double maxValue = DBL_MIN;
    for (PlotableItem* item : itemVector[canvasId]) {
        maxValue = qMax(maxValue, item->regionalMaxValue(minIndex, maxIndex));
    }
    return maxValue;
}

double SplitterPlot::regionalMinValue(int minIndex, int maxIndex, int canvasId) const {
    double minValue = DBL_MAX;
    for (PlotableItem* item : itemVector[canvasId]) {
        minValue = qMin(minValue, item->regionalMinValue(minIndex, maxIndex));
    }
    return minValue;
}

void SplitterPlot::drawCanvas(QWidget *canvas, QPainter *painter)
{
    drawCanvasGrid(canvas, painter);

    int offset = 257;
    mItemAxis->setIndexRange(0, offset);
    QwtScaleMap map = mItemAxis->caculateCanvasMap(canvas);

    // 绘制元素中心所在的位置的横坐标QVector。
    QVector<int> itemPosXVector;
    itemPosXVector.resize(offset);
    for (int i = 0; i < offset; i++) {
        itemPosXVector[i] = map.transform(i);
    }

    int canvasIndex = splitterCanvas->indexOfCanvas(canvas);
    if(itemVector.size() > canvasIndex && canvasIndex >= 0){
        QwtScaleMap cMap = canvasMap(canvasIndex, AbstractPlot::yRight);
        const QList<PlotableItem*>& list = itemVector[canvasIndex];
        for (auto item : list) {
            if(item->size() == 0) return;
            item->draw(itemPosXVector, {0, offset}, 1, cMap, painter);
        }
    }
}

void SplitterPlot::drawCanvasGrid(QWidget *canvas, QPainter *painter)
{
    painter->save();

    QPen gridLinePen(QColor("#800000"));

    QPen borderPen(QColor("#800000"));

    int canvasIndex = splitterCanvas->indexOfCanvas(canvas);

    const QwtScaleDiv* div;
    if(orientation() == Qt::Vertical){
        div = &((SplitterScaleWidget*)(axisArray[yLeft]))->scaleDraw(canvasIndex)->scaleDiv();
    }else{
        div = &((SplitterScaleWidget*)(axisArray[xTop]))->scaleDraw(canvasIndex)->scaleDiv();
    }

    QwtScaleMap map = canvasMap(canvasIndex, yLeft);

    const QList<double>& tickList = div->ticks(QwtScaleDiv::MajorTick);

    int listSize = tickList.size();
    int counter = 0;

    if(canvasIndex != 0){
        painter->setPen(borderPen);
        painter->drawLine(0, 0, canvas->width() - 1, 0);
    }

    for(double value : tickList){
        int y = map.transform(value);
        if(counter == 0 && canvasIndex != getCanvasCount() - 1){
            painter->setPen(borderPen);
        }else if(counter == listSize - 1 && canvasIndex != getCanvasCount() - 1){
            painter->setPen(borderPen);
        }else{
            painter->setPen(gridLinePen);
        }
        painter->drawLine(0, y, canvas->width() - 1, y);
        counter++;
    }

    painter->restore();
}

int SplitterPlot::dataSize(int canvasIndex)
{
    return itemVector[canvasIndex].at(0)->size();
}

void SplitterPlot::onTargetMove(QPoint mouseGlobalPos){
    mouseGlobalPos = caculateAndUpdateTracerPos(mouseGlobalPos);

    splitterCanvas->setTracerGlobalPos(mouseGlobalPos);

    if(getTraceAxis(xTop)->isAxisEnabled()){
        getTraceAxis(xTop)->setTracerLocalPos(getTraceAxis(xTop)->mapFromGlobal(mouseGlobalPos).x());
    }
    if(getTraceAxis(xBottom)->isAxisEnabled()){
        getTraceAxis(xBottom)->setTracerLocalPos(getTraceAxis(xBottom)->mapFromGlobal(mouseGlobalPos).x());
    }
    if(getTraceAxis(yRight)->isAxisEnabled()){
        getTraceAxis(yRight)->setTracerLocalPos(getTraceAxis(yRight)->mapFromGlobal(mouseGlobalPos).y());
    }
    if(getTraceAxis(yLeft)->isAxisEnabled()){
        getTraceAxis(yLeft)->setTracerLocalPos(getTraceAxis(yLeft)->mapFromGlobal(mouseGlobalPos).y());
    }
}

void SplitterPlot::onTargetLeave(){
    splitterCanvas->clearTracer();
    if(getTraceAxis(xTop)->isAxisEnabled()){
        getTraceAxis(xTop)->onTargetLeaved();
    }
    if(getTraceAxis(xBottom)->isAxisEnabled()){
        getTraceAxis(xBottom)->onTargetLeaved();
    }
    if(getTraceAxis(yRight)->isAxisEnabled()){
        getTraceAxis(yRight)->onTargetLeaved();
    }
    if(getTraceAxis(yLeft)->isAxisEnabled()){
        getTraceAxis(yLeft)->onTargetLeaved();
    }
}

void SplitterPlot::onCanvasResized(int index)
{
    if(orientation() == Qt::Vertical){
        updateAxis(index);
        ((SplitterScaleWidget*)axisArray[yLeft])->updateScaleDiv(index);
        ((SplitterScaleWidget*)axisArray[yRight])->updateScaleDiv(index);
    }else{

    }
    replot();
}

void SplitterPlot::onCanvasResized()
{
    updateAxes();
}

QPoint SplitterPlot::caculateAndUpdateTracerPos(QPoint globalPos){
    return globalPos;
}

void SplitterPlot::showEvent(QShowEvent *event)
{
    AbstractTracePlot::showEvent(event);
    updateAxes();
    replot();
}

void SplitterPlot::resizeEvent(QResizeEvent *event)
{
    AbstractTracePlot::resizeEvent(event);
    updateAxes();
}

RealTimeItemAxis *SplitterPlot::itemAxis()
{
    return mItemAxis;
}

}
