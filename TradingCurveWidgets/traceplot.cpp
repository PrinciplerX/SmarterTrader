#include "traceplot.h"
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include "abstractplotlayout.h"

namespace TradingCurveWidgets{

TracePlot::TracePlot(QWidget *parent):
    SimplePlot(parent),
    traceCanvas(new SplitterPlotCanvas())
{
    init();
    this->tracerSetting={QPen(Qt::black)};
    tracerSetting.pen.setWidth(1);
}

TracePlot::TracePlot(TracePlotCanvas::TracerSetting tracerSetting, QWidget *parent):
    SimplePlot(parent),
    traceCanvas(new SplitterPlotCanvas())
{
    init();
    this->tracerSetting=tracerSetting;
}

void TracePlot::init()
{
    tTsw=new TraceScaleWidget(AbstractAxis::AxisType::TopScale);
    bTsw=new TraceScaleWidget(AbstractAxis::AxisType::BottomScale);
    lTsw=new TraceScaleWidget(AbstractAxis::AxisType::LeftScale);
    rTsw=new TraceScaleWidget(AbstractAxis::AxisType::RightScale);

    /*! \todo setPlot并不是一个非常妥当的解决方案 */
    traceCanvas->setPlot(this);
    this->setCanvas(traceCanvas);

    this->getPlotLayout()->setCanvasMargin(0);
    this->setAxisWidget(SimplePlot::Axis::xTop,tTsw);
    this->setAxisWidget(SimplePlot::Axis::xBottom,bTsw);
    this->setAxisWidget(SimplePlot::Axis::yLeft,lTsw);
    this->setAxisWidget(SimplePlot::Axis::yRight,rTsw);

    //保证canvas被四面坐标的backbone部分完美贴合。
    //this->plotLayout()->setAlignCanvasToScales(true);
    this->setTracking(true);

    connect(traceCanvas->getTraceCanvas(),SIGNAL(targetMoved(QPoint)),this,SLOT(onTargetMove(QPoint)));
    connect(traceCanvas->getTraceCanvas(),SIGNAL(targetLeaved()),this,SLOT(onTargetLeave()));
}

QPoint TracePlot::caculateAndUpdateTracerPos(QPoint mousePosInCanvas){
    return mousePosInCanvas;
}

/*!
 * \brief 鼠标移动处理函数。
 * \param mouseGlobalPos 鼠标的当前全局位置。
 */
void TracePlot::onTargetMove(QPoint mouseGlobalPos){
    setTracerGlobalPos(mouseGlobalPos);
    emit targetMoved(mouseGlobalPos);
}

void TracePlot::onTargetLeave(){
    hideTracer();
    emit targetLeaved();
}

void TracePlot::hideAxisTick(){
    tTsw->setTickLength(0,0,0);
    bTsw->setTickLength(0,0,0);
    rTsw->setTickLength(0,0,0);
    lTsw->setTickLength(0,0,0);
}

const QWidget *TracePlot::getCanvas() const {
    return traceCanvas->getTraceCanvas();
}

void TracePlot::setTitleWidget(QWidget *titleWidget){
    titleWidget->setFixedHeight(15);
    traceCanvas->insertWidget(0, titleWidget);
    traceCanvas->handle(1)->setEnabled(false);
    traceCanvas->handle(1)->setCursor(Qt::ArrowCursor);
}

QWidget *TracePlot::getTitleWidget()
{
    //! \todo 0位置上不一定是titleWidget
    return traceCanvas->widget(0);
}

void TracePlot::setTracking(bool enable){
    traceCanvas->getTraceCanvas()->setTracking(enable);
    tTsw->enableTracking(enable);
    bTsw->enableTracking(enable);
    rTsw->enableTracking(enable);
    lTsw->enableTracking(enable);
}

void TracePlot::setScaleWidget(SimplePlot::Axis axis, TraceScaleWidget *scaleWidget){
    switch(axis){
    case AbstractPlot::xBottom:
        scaleWidget->setTracerSetting(bTsw->getTracerSetting());
        bTsw = scaleWidget;
        break;
    case AbstractPlot::xTop:
        scaleWidget->setTracerSetting(tTsw->getTracerSetting());
        tTsw = scaleWidget;
        break;
    case AbstractPlot::yLeft:
        scaleWidget->setTracerSetting(lTsw->getTracerSetting());
        lTsw = scaleWidget;
        break;
    case AbstractPlot::yRight:
        scaleWidget->setTracerSetting(rTsw->getTracerSetting());
        rTsw = scaleWidget;
        break;
    }
    //由SimplePlot删除坐标轴控件。
    setAxisWidget(axis,scaleWidget);
}

TraceScaleWidget *TracePlot::getScaleWidget(SimplePlot::Axis axis){
    switch(axis){
    case AbstractPlot::xBottom:   return bTsw;
    case AbstractPlot::xTop:      return tTsw;
    case AbstractPlot::yLeft:     return lTsw;
    case AbstractPlot::yRight:    return rTsw;
    }
    return nullptr;
}

void TracePlot::setAxisTracerSetting(SimplePlot::Axis axis, TraceScaleWidget::TracerSetting setting){
    switch(axis){
    case AbstractPlot::xBottom:bTsw->setTracerSetting(setting);break;
    case AbstractPlot::xTop:tTsw->setTracerSetting(setting);break;
    case AbstractPlot::yLeft:lTsw->setTracerSetting(setting);break;
    case AbstractPlot::yRight:rTsw->setTracerSetting(setting);break;
    }
}

void TracePlot::setAxisTracerSetting(TraceScaleWidget::TracerSetting setting){
    bTsw->setTracerSetting(setting);
    tTsw->setTracerSetting(setting);
    lTsw->setTracerSetting(setting);
    rTsw->setTracerSetting(setting);
}

void TracePlot::setTracerCanvasPosDirectly(QPoint pos){
    setTracerGlobalPosDirectly(traceCanvas->getTraceCanvas()->mapToGlobal(pos));
}

void TracePlot::setTracerGlobalPosDirectly(QPoint pos){
    traceCanvas->getTraceCanvas()->setTracerPos(traceCanvas->getTraceCanvas()->mapFromGlobal(pos));
    QPoint tracerGlobalPos = pos;

    tTsw->setTracerLocalPos(tTsw->mapFromGlobal(tracerGlobalPos).x());
    bTsw->setTracerLocalPos(bTsw->mapFromGlobal(tracerGlobalPos).x());
    lTsw->setTracerLocalPos(lTsw->mapFromGlobal(tracerGlobalPos).y());
    rTsw->setTracerLocalPos(rTsw->mapFromGlobal(tracerGlobalPos).y());
}

void TracePlot::setTracerCanvasPos(QPoint pos){
    QPoint tracerPosInCanvas=caculateAndUpdateTracerPos(pos);
    setTracerCanvasPosDirectly(tracerPosInCanvas);
}
void TracePlot::setTracerGlobalPos(QPoint pos){
    setTracerCanvasPos(getCanvas()->mapFromGlobal(pos));
}

TraceScaleWidget *TracePlot::getTraceScaleWidget(SimplePlot::Axis axisId){
    switch(axisId){
    case AbstractPlot::xTop:return tTsw;
    case AbstractPlot::yLeft:return lTsw;
    case AbstractPlot::yRight:return rTsw;
    case AbstractPlot::xBottom:return bTsw;
    default:return nullptr;
    }
}

void TracePlot::hideTracer(){
    tTsw->onTargetLeaved();
    bTsw->onTargetLeaved();
    lTsw->onTargetLeaved();
    rTsw->onTargetLeaved();
    traceCanvas->getTraceCanvas()->setTracerPos(QPoint(-1,-1));
}

}
