#include "abstracttraceaxis.h"

AbstractTraceAxis::AbstractTraceAxis(QWidget *parent):
    AbstractAxis(parent)
{

}

AbstractTraceAxis::AbstractTraceAxis(AbstractAxis::AxisType axisType, QWidget *parent):
    AbstractAxis(axisType, parent)
{

}

void AbstractTraceAxis::enableTracking(bool enable){
    tracking = enable;
    update();
}

bool AbstractTraceAxis::isTrackingEnabled() const {
    return tracking;
}

void AbstractTraceAxis::setTracerLocalPos(int pos){
    tracerPosition = pos;
    onTargetMoved(pos);
    if(isTrackingEnabled())
        update();
}

int AbstractTraceAxis::getTracerPos() const {
    return tracerPosition;
}

void AbstractTraceAxis::onTargetMoved(int pos) {}

void AbstractTraceAxis::onTargetMoved(QPoint globalPos)
{
    QPoint localPos = mapFromGlobal(globalPos);
    int tracerPos;

    switch(getType()){
    case AbstractTraceAxis::TopScale:
    case AbstractTraceAxis::BottomScale:
        tracerPos = localPos.x();
        break;
    case AbstractTraceAxis::LeftScale:
    case AbstractTraceAxis::RightScale:
        tracerPos = localPos.y();
        break;
    default:
        tracerPos = -1;
        qDebug() << "AbstractTraceAxis::onTargetMoved: 出现了未知的坐标轴类型。";
    }

    setTracerLocalPos(tracerPos);
}

void AbstractTraceAxis::onTargetLeaved(){
    setTracerLocalPos(-1);
}

void AbstractTraceAxis::drawTracer(QPainter *painter, int tracerPos) const
{
    Q_UNUSED(painter);
    Q_UNUSED(tracerPos);
    qDebug() << "游标跟踪功能被启用，但是并没有绘制游标。";
}

void AbstractTraceAxis::drawContents(QPainter *painter) const {
    drawAxis(painter);
    if(isTrackingEnabled() && tracerPosition >= 0){
        drawTracer(painter, tracerPosition);
    }
}
