#include "splitterplotcanvas.h"
#include "simpleplot.h"
#include "abstractplot.h"
#include <QCursor>

namespace TradingCurveWidgets{

SplitterPlotCanvas::SplitterPlotCanvas(Qt::Orientation orientation, int canvasCount, AbstractPlot *parent):
    QSplitter(orientation, parent),
    plot(parent)
{
    for(int i = 0; i < canvasCount; i++){
        appendCanvas();
    }

    // this->setStyleSheet("QSplitter::handle { background: darkgray;}");

    setChildrenCollapsible(false);
    setHandleWidth(0);
}

SplitterPlotCanvas::~SplitterPlotCanvas() {}

void SplitterPlotCanvas::setPlot(AbstractPlot *plot){
    for(int i = 0; i < canvasVector.size(); i++){
        canvasVector[i]->setPlot(plot);
    }
}

/*!
 * \brief 查询画布的序号。注意这里的序号是相对于画布控件而言的，和其他类型的控件无关。
 * \param canvas 要查询的画布控件。
 * \return 如果找到了该画布，则返回画布序号；未找到则返回-1。
 * \sa indexOf()
 */
int SplitterPlotCanvas::indexOfCanvas(QWidget *canvas) const
{
    for(int i = 0; i < canvasVector.size(); i++){
        if(canvasVector[i] == canvas){
            return i;
        }
    }
    return -1;
}

void SplitterPlotCanvas::setCanvasBackground(const QBrush &brush)
{
    for(int i = 0; i < getCanvasCount(); i++){
        setCanvasBackground(brush, i);
    }
}

void SplitterPlotCanvas::setCanvasBackground(const QBrush &brush, int index)
{
    QPalette pal = getCanvasAt(index)->palette();
    pal.setBrush( QPalette::Window, brush );
    getCanvasAt(index)->setPalette( pal );
}

void SplitterPlotCanvas::setTracerLocalPos(QPoint tracerPos)
{
    for(int i = 0; i < canvasVector.size(); i++){
        canvasVector[i]->setTracerPos(canvasVector[i]->mapFromParent(tracerPos));
    }
}

void SplitterPlotCanvas::setTracerGlobalPos(QPoint tracerPos)
{
    for(int i = 0; i < canvasVector.size(); i++){
        canvasVector[i]->setTracerPos(canvasVector[i]->mapFromGlobal(tracerPos));
    }
}

void SplitterPlotCanvas::clearTracer()
{
    for(int i = 0; i < canvasVector.size(); i++){
        canvasVector[i]->clearTracer();
    }
}

void SplitterPlotCanvas::appendCanvas()
{
    TracePlotCanvas* canvas = new TracePlotCanvas(plot);
    canvas->installEventFilter(this);
    canvasVector.append(canvas);
    addWidget(canvas);

    connect(canvas, SIGNAL(targetMoved(QPoint)), this, SIGNAL(targetMoved(QPoint)));
    connect(canvas, SIGNAL(targetLeaved()), this, SIGNAL(targetLeaved()));
}

void SplitterPlotCanvas::replot(){
    for(int i = 0; i < canvasVector.size(); i++){
        canvasVector[i]->replot();
    }
}

bool SplitterPlotCanvas::eventFilter(QObject* obj, QEvent* event){
    if(event->type() == QEvent::Resize){
        for(int i = 0; i < canvasVector.size(); i++){
            if(obj == canvasVector[i]){
                emit canvasResized(i);
                break;
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

void SplitterPlotCanvas::resizeEvent(QResizeEvent *event)
{
    QSplitter::resizeEvent(event);
    emit canvasResized();
}

}
