#include "traceplotcanvas.h"
#include <QPainter>
#include <QPen>
#include "simpleplot.h"
#include <QDebug>

namespace TradingCurveWidgets{

TracePlotCanvas::TracePlotCanvas(AbstractPlot *plot )
    :SimplePlotCanvas(plot)
{
    setTracking(true);
    setFrameStyle(QFrame::Plain);
    setCursor(QCursor());

    this->tracerSetting={QPen(Qt::black)};
    tracerSetting.pen.setWidth(1);
}

TracePlotCanvas::TracePlotCanvas(TracePlotCanvas::TracerSetting setting, AbstractPlot *parent):
    SimplePlotCanvas(parent)
{
    setTracking(true);
    setFrameStyle(QFrame::Plain);
    setCursor(QCursor());

    this->tracerSetting=setting;
}

void TracePlotCanvas::mouseMoveEvent(QMouseEvent* event){
    SimplePlotCanvas::mouseMoveEvent(event);
    emit targetMoved(event->globalPos());
}

void TracePlotCanvas::setTracking(bool enable){
    tracking=enable;
    setMouseTracking(enable);
}

void TracePlotCanvas::setTracerPos(QPoint tracerPos)
{
    this->tracerPos=tracerPos;
    update();
}

void TracePlotCanvas::clearTracer(){
    setTracerPos(QPoint(-1, -1));
}

void TracePlotCanvas::paintEvent(QPaintEvent* event)
{
    SimplePlotCanvas::paintEvent(event);
    QPainter painter(this);
    if(tracking){
        painter.setPen(tracerSetting.pen);
        //painter.setBrush(palette().windowText());
        painter.drawLine(0,tracerPos.y(),this->width(),tracerPos.y());
        painter.drawLine(tracerPos.x(),0,tracerPos.x(),this->height());
    }
}

void TracePlotCanvas::leaveEvent(QEvent* event){
    tracerPos=QPoint(-1,-1);
    Q_UNUSED(event);
    update();
    emit targetLeaved();
}

}
