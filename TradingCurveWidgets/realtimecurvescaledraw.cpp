#include "realtimecurvescaledraw.h"
#include <qwt_text.h>

namespace TradingCurveWidgets {

RealTimeCurveScaleDraw::RealTimeCurveScaleDraw(double centralValue)
{
    this->centralValue = centralValue;
}

void RealTimeCurveScaleDraw::drawLabel(QPainter *painter, double value) const
{
    QwtText lbl = tickLabel( painter->font(), value );
    if ( lbl.isEmpty() )
        return;

    QPointF pos = labelPosition( value );
    QSizeF labelSize = lbl.textSize( painter->font() );

    const QTransform transform = labelTransformation( pos, labelSize );

    /*********************************************************************************************
    QRect mapedRect = transform.mapRect(QRect(QPoint(0,0), labelSize.toSize()));
    if(mapedRect.top() < 0 || mapedRect.bottom() < 0 || mapedRect.left() < 0 || mapedRect.right() < 0){
        return;
    }
    *********************************************************************************************/

    painter->save();

    painter->setWorldTransform( transform, true );

    if(value > centralValue){
        painter->setPen(QColor("#FF3232"));
    }else if(value < centralValue){
        painter->setPen(QColor("#00E600"));
    }else{
        painter->setPen(Qt::white);
    }

    lbl.draw ( painter, QRect( QPoint( 0, 0 ), labelSize.toSize() ) );

    painter->restore();
}

void RealTimeCurveScaleDraw::setBaseValue(double baseValue)
{
    centralValue = baseValue;
}

}
