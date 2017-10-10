#include "percentagescaledraw.h"

namespace TradingCurveWidgets {

PercentageScaleDraw::PercentageScaleDraw(double value):
    mBaseValue(value)
{
}

QwtText PercentageScaleDraw::label(double value) const {
    double ratio = value / mBaseValue;
    double percentage = (ratio - 1.0) * 100;

    return QString("%1%").arg(QString::number(qAbs(percentage), 'f', 2));
}

void PercentageScaleDraw::drawLabel(QPainter *painter, double value) const
{
    QwtText lbl = tickLabel( painter->font(), value );
    if ( lbl.isEmpty() )
        return;

    QPointF pos = labelPosition( value );
    QSizeF labelSize = lbl.textSize( painter->font() );

    const QTransform transform = labelTransformation( pos, labelSize );

    /**********
    QRect mapedRect = transform.mapRect(QRect(QPoint(0,0), labelSize.toSize()));
    if(mapedRect.top() < 0 || mapedRect.bottom() < 0 || mapedRect.left() < 0 || mapedRect.right() < 0){
        return;
    }
    **********/

    painter->save();

    painter->setWorldTransform( transform, true );

    if(value > mBaseValue){
        painter->setPen(QColor("#FF3232"));
    }else if(value < mBaseValue){
        painter->setPen(QColor("#00E600"));
    }else{
        painter->setPen(Qt::white);
    }

    lbl.draw ( painter, QRect( QPoint( 0, 0 ), labelSize.toSize() ) );

    painter->restore();
}

void PercentageScaleDraw::setBaseValue(double value)
{
    mBaseValue = value;
}

double PercentageScaleDraw::baseValue() const
{
    return mBaseValue;
}

}
