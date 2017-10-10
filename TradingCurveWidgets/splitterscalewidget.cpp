#include "splitterscalewidget.h"
#include "qwt_painter.h"
#include "qwt_color_map.h"
#include "qwt_scale_map.h"
#include "qwt_math.h"
#include "qwt_scale_div.h"
#include "qwt_text.h"
#include "qwt_scale_engine.h"
#include "splitterplot.h"
#include <QPainter>
#include <QEvent>
#include <QtMath>
#include <QStyle>
#include <QStyleOption>

namespace TradingCurveWidgets{

SplitterScaleWidget::SplitterScaleWidget(AbstractAxis::AxisType align, int axisCount, SplitterPlot *parent ):
    AbstractTraceAxis( align, parent ),
    plot(parent)
{
    this->enableTracking(true);

    for(int i = 0; i < axisCount; i++){
        appendAxis();
    }
}

SplitterScaleWidget::SplitterScaleWidget(TraceScaleWidget *mainAxis, int axisCount, SplitterPlot *parent):
    AbstractTraceAxis( mainAxis->getType(), parent ),
    plot(parent)
{
    this->enableTracking(true);
    appendAxis(mainAxis);
    for(int i = 0; i < axisCount - 1; i++){
        appendAxis();
    }
}

/*!
 * \brief 绘制坐标轴。
 * \param painter 画笔。
 */
void SplitterScaleWidget::drawAxis( QPainter *painter ) const
{
    for(int i = 0; i < axesVector.size(); i++){
        painter->save();
        painter->translate(axesVector[i]->x(), axesVector[i]->y());
        axesVector[i]->drawAxis(painter);
        painter->restore();
    }
}

void SplitterScaleWidget::drawTracer(QPainter *painter, int tracerPos) const
{
    if(getType() == LeftScale || getType() == RightScale){
        for(int i = 0; i < axesVector.size(); i++){
            if(axesVector[i]->geometry().contains(0, tracerPos)){
                painter->save();
                painter->translate(axesVector[i]->x(), axesVector[i]->y());
                axesVector[i]->drawTracer(painter, tracerPos - axesVector[i]->y());
                painter->restore();
                return;
            }
        }
    }else{
        for(int i = 0; i < axesVector.size(); i++){
            if(axesVector[i]->geometry().contains(tracerPos, 0)){
                painter->save();
                painter->translate(axesVector[i]->x(), axesVector[i]->y());
                axesVector[i]->drawTracer(painter, tracerPos - axesVector[i]->x());
                painter->restore();
                return;
            }
        }
    }
}

void SplitterScaleWidget::setValueRange(double lowerBound, double upperBound, int axisId){
    axesVector[axisId]->setCanvasValueRange(lowerBound, upperBound, 0);
}

void SplitterScaleWidget::setAxisPalette(const QPalette &palette, int axisIndex)
{
    axesVector[axisIndex]->setPalette(palette);
}

const RectangularAxisDraw* SplitterScaleWidget::scaleDraw(int index) const
{
    return axesVector[index]->scaleDraw();
}

RectangularAxisDraw *SplitterScaleWidget::scaleDraw(int index)
{
    return axesVector[index]->scaleDraw();
}

void SplitterScaleWidget::setScaleDraw(RectangularAxisDraw *scaleDraw, int index)
{
    axesVector[index]->setScaleDraw(scaleDraw);
}

const QwtScaleMap &SplitterScaleWidget::canvasMap(int index) const {
    return canvasMapVector[index];
}

QwtScaleMap SplitterScaleWidget::caculateCanvasMap(int index) const
{
    QwtScaleMap map;
    if(plot->getCanvasCount() <= index || index < 0 )
        return map;

    map.setTransformation( axesVector[index]->getScaleEngine()->transformation() );
    map.setScaleInterval( axesVector[index]->getCanvasMinValue(), axesVector[index]->getCanvasMaxValue());

    const TraceScaleWidget *s = axesVector[index];

    if ( getType() == LeftScale || getType() == RightScale ){
        double y = s->y() + s->getStartBorderDist() - plot->getCanvasAt(index)->y();
        double h = s->height() - s->getStartBorderDist() - s->getEndBorderDist();
        map.setPaintInterval( y + h, y );
    }else{
        double x = s->x() + s->getStartBorderDist() - plot->getCanvasAt(index)->x();
        double w = s->width() - s->getStartBorderDist() - s->getEndBorderDist();
        map.setPaintInterval( x, x + w );
    }

    return map;
}

/*!
 * \brief 基于当前的几何大小和字体计算坐标坐标轴的几何大小与布局（geometry and layout）。
 * \param update_geometry 通知布局系统更新并且重绘坐标轴。
 */
void SplitterScaleWidget::updateLayout( bool update_geometry ){
    for(int i = 0; i < axesVector.size(); i++){
        axesVector[i]->updateLayout(false);
    }
    if(update_geometry){
        update();
    }
}

void SplitterScaleWidget::appendAxis()
{
    TraceScaleWidget* axis = new TraceScaleWidget(getType(), this);
    axis->installEventFilter(this);
    axis->setFixedBorderDist(0, 0);
    axesVector.append(axis);
}

void SplitterScaleWidget::appendAxis(TraceScaleWidget *axis)
{
    if(!axis) return;

    if(axis->getType() != this->getType()){
        axis->setAlignment(getType());
    }

    axis->setParent(this);
    axis->installEventFilter(this);
    axis->setFixedBorderDist(0, 0);
    axesVector.append(axis);
}

void SplitterScaleWidget::onCanvasResized(int index)
{
    if(index < 0 || index >= axesVector.size())
        return;
    const TracePlotCanvas* canvas = plot->getCanvasAt(index);
    TraceScaleWidget* axis = axesVector[index];
    QPoint startPoint;
    int scaleRangeOffset;
    int dim;

    if(plot->orientation() == Qt::Vertical){
        dim = width();
        scaleRangeOffset = canvas->height();

        startPoint = mapFromGlobal(canvas->mapToGlobal(QPoint(0, canvas->contentsRect().bottom() + 1)));
        if(getType() == LeftScale){
            startPoint.setX(contentsRect().right() + 1);
        }else{
            startPoint.setX(contentsRect().left());
        }
    }else{
        dim = height();
        scaleRangeOffset = canvas->width();

        startPoint = mapFromGlobal(canvas->mapToGlobal(QPoint(0, canvas->contentsRect().left())));
        if(getType() == TopScale){
            startPoint.setY(contentsRect().bottom() + 1);
        }else{
            startPoint.setY(contentsRect().top());
        }
    }
    axis->setGeometry(axis->rectHint(startPoint, scaleRangeOffset, dim));
    update();
}

bool SplitterScaleWidget::eventFilter(QObject* obj, QEvent* event)
{
    if(event->type() == QEvent::Paint){
        if(axesVector.contains((TraceScaleWidget*)obj)){
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void SplitterScaleWidget::onTargetMoved(int pos)
{
    if(getType() == LeftScale || getType() == RightScale){
        for(int i = 0; i < axesVector.size(); i++){
            QPoint p(0, pos);
            if(axesVector[i]->geometry().contains(p)){
                axesVector[i]->setTracerLocalPos(axesVector[i]->mapFrom(this, p).y());
                return;
            }
        }
    }else{
        for(int i = 0; i < axesVector.size(); i++){
            QPoint p(pos, 0);
            if(axesVector[i]->geometry().contains(p)){
                axesVector[i]->setTracerLocalPos(axesVector[i]->mapFrom(this, p).x());
                return;
            }
        }
    }
}

/*!
  \brief Notify a change of the scale

  This virtual function can be overloaded by derived
  classes. The default implementation updates the geometry
  and repaints the widget.
*/
void SplitterScaleWidget::scaleChange(){
    updateLayout();
}

int SplitterScaleWidget::dimHint() const{
    int result = 0;
    for(int i = 0; i < axesVector.size(); i++){
        result = qMax(result, axesVector[i]->dimHint());
    }
    return result;
}

void SplitterScaleWidget::setTickLength(int minorTickLength, int mediumTickLength, int majortickLength)
{
    for(int i = 0; i < axesVector.size(); i++){
        axesVector[i]->setTickLength(minorTickLength, mediumTickLength, majortickLength);
    }
}

void SplitterScaleWidget::getBorderDistHint( int &start, int &end ) const {
    int sstart = 0;
    int send = 0;
    int estart = 0;
    int eend = 0;

    axesVector[0]->getBorderDistHint( sstart, send );
    axesVector[axesVector.size() - 1]->getBorderDistHint( estart, eend);
    start = sstart;
    end = eend;
}

void SplitterScaleWidget::updateScaleDiv(int index){
    TraceScaleWidget* axis = axesVector[index];

    double min, max;
    axis->getCanvasValueRange(min, max);
    int length = axis->scaleDraw()->length();

    if(index == 0){
        axis->setScaleDiv(ValueAxisHelper::caculateBondScaleDiv(min, max, length, 20));
    }else{
        axis->setScaleDiv(ValueAxisHelper::caculateMidScaleDiv(min, max, length, 20));
    }

    /*************************************
    if(length < 3) return;

    double margin = 0.5 * (max - min) / (length - 1);

    int intervalCount = qMax(length / 20, 2);

    double interval = (max - min) / intervalCount;

    //先添加顶部的tick，再将下面的空间等分确认剩余的tick
    QList<double> ticks[QwtScaleDiv::NTickTypes];
    if(index == 0) ticks[QwtScaleDiv::MajorTick].append(min);

    for(int i = 1; i < intervalCount; i++){
        ticks[QwtScaleDiv::MajorTick].append(min + interval * i);
    }

    if(index == 0) ticks[QwtScaleDiv::MajorTick].append(max);
    axis->setScaleDiv(QwtScaleDiv(min - margin, max + margin, ticks));
    **************************************/
}

}
