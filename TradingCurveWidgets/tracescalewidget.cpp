#include "tracescalewidget.h"
#include <QMessageBox>
#include <qwt_scale_map.h>

namespace TradingCurveWidgets{

TraceScaleWidget::TraceScaleWidget(AbstractAxis::AxisType align, QWidget *parent):
    SimpleScaleWidget(align,parent)
{
    enableTracking(true);
    setMargin(0);
    tracerSetting={QPen(Qt::black),QBrush(Qt::black),QPen(Qt::white)};
}

TraceScaleWidget::~TraceScaleWidget(){}

void TraceScaleWidget::drawTracerLabel(QPainter *painter, double val) const{
    scaleDraw()->drawLabel(painter, val);
}

void TraceScaleWidget::drawTracer(QPainter *painter, int tracerPos) const {

    //将位置转化为真实数值
    double value = scaleDraw()->scaleMap().invTransform((double)tracerPos);

    painter->save();
    painter->setPen(tracerSetting.tracerPen);
    painter->setBrush(tracerSetting.tracerBrush);

    QwtText lbl = scaleDraw()->label(value);

    lbl.setRenderFlags( 0 );
    lbl.setLayoutAttribute( QwtText::MinimumLayout );

    ( void )lbl.textSize( painter->font() ); // initialize the internal cache

    if ( lbl.isEmpty() )
        return;

    QPointF pos = scaleDraw()->labelPosition( value );
    QSizeF labelSize = lbl.textSize( painter->font() );

    //scaleDraw()->drawTick(painter, value, scaleDraw()->tickLength(QwtScaleDiv::MajorTick));

    //绘制指向label的短直线
    //绘制label所在区域的边框
    painter->setBrush(palette().highlight());

    switch(this->alignment()){
    case AbstractAxis::AxisType::TopScale:
        painter->drawLine(getTracerPos(), height(),
                          getTracerPos(), height() - scaleDraw()->maxTickLength());

        painter->drawRect(pos.x() - labelSize.width()/2.0 - getTracerLabelMargin(),
                          0,
                          labelSize.width() + 2*getTracerLabelMargin(),
                          height() - getTracerLabelMargin() - 0.5*painter->pen().widthF() - scaleDraw()->maxTickLength());
        break;
    case AbstractAxis::AxisType::BottomScale:
        painter->drawLine(getTracerPos(), 0,
                          getTracerPos(), scaleDraw()->maxTickLength());

        painter->drawRect(pos.x() - labelSize.width()/2.0 - getTracerLabelMargin(),
                          getTracerLabelMargin() + 0.5 * painter->pen().widthF() + scaleDraw()->maxTickLength(),
                          labelSize.width() + 2*getTracerLabelMargin(),
                          height() - getTracerLabelMargin() - 0.5*painter->pen().widthF() - scaleDraw()->maxTickLength());
        break;
    case AbstractAxis::AxisType::LeftScale:
        painter->drawLine(width(), getTracerPos(),
                          width() - scaleDraw()->maxTickLength(), getTracerPos());

        painter->drawRect(0,
                          pos.y() - labelSize.height()/2.0 - getTracerLabelMargin(),
                          width() - 0.5* painter->pen().width() - scaleDraw()->maxTickLength(),
                          labelSize.height() + 2*getTracerLabelMargin());
        break;
    case AbstractAxis::AxisType::RightScale:
        painter->drawLine(0, getTracerPos(),
                          scaleDraw()->maxTickLength(), getTracerPos());

        painter->drawRect(0.5 * painter->pen().widthF() + scaleDraw()->maxTickLength(),
                          pos.y() - labelSize.height()/2.0 - getTracerLabelMargin(),
                          width() - scaleDraw()->maxTickLength() - 0.5 * painter->pen().widthF(),
                          labelSize.height() + 2*getTracerLabelMargin());
        break;
    }

    painter->setPen(palette().highlightedText().color());
    painter->setBrush(palette().highlightedText());
    drawTracerLabel(painter, value);

    ///////////////////////////////////////////////////////////////////
    //painter->drawRect(QRect( QPoint( 0, 0 ), labelSize.toSize() ));//
    ///////////////////////////////////////////////////////////////////

    painter->restore();
}

void TraceScaleWidget::updateScaleDiv(){
    /*
    if(alignment() == AbstractAxis::LeftScale || alignment() == AbstractAxis::RightScale){
        int step = height() / maxMajorIntervalSpace;

        double min, max;
        getValueRange(min, max);
        double stepSize=qAbs((max - min) / step);

        if(step < 2){
            step = 2;
            stepSize = max - min;
        }

        setScaleDiv(TradingCurveScaleEngine().divideScale(min, max, step, 0, stepSize));
    }
    */
}

void TraceScaleWidget::setTickLength(int minorTickLength,
                                     int mediumTickLength,
                                     int majortickLength){
    scaleDraw()->setTickLength(QwtScaleDiv::TickType::MajorTick,minorTickLength);
    scaleDraw()->setTickLength(QwtScaleDiv::TickType::MediumTick,mediumTickLength);
    scaleDraw()->setTickLength(QwtScaleDiv::TickType::MinorTick,majortickLength);
}

}
