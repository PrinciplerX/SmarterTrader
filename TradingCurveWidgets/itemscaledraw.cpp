#include "itemscaledraw.h"
#include "abstractitemaxis.h"
#include "qwt_painter.h"
#include <QDebug>
#include <QtMath>
#include <cfloat>

namespace TradingCurveWidgets {

ItemScaleDraw::ItemScaleDraw(AbstractItemAxis *axis):
    itemAxis(axis)
{
}

QwtText ItemScaleDraw::label(double value) const {
    int index = qRound(scaleMap().transform(value));

    // qDebug() << QString("ItemScaleDraw::label: (x, index) = (%1, %2)").arg(QString::number(x), QString::number(index));

    return itemAxis->getTickLabel(index);
}

void ItemScaleDraw::draw(QPainter *painter, const QPalette &palette) const
{
    painter->save();

    QPen pen = painter->pen();
    pen.setWidth( penWidth() );
    pen.setCosmetic( false );
    painter->setPen( pen );

    QVector<int> indexPos;
    QVector<QString> indexLabel;
    const QMap<int, QString>& indexMap = itemAxis->getMajorTickSeries();

    indexPos.resize(indexMap.size());
    indexLabel.resize(indexMap.size());

    int counter = 0;
    for(QMap<int, QString>::const_iterator i = indexMap.begin(); i != indexMap.end(); i++){
        // indexMap -
        indexPos[counter] = itemAxis->caculatePlotLocalPos(i.key());
        indexLabel[counter] = i.value();
        counter++;
    }

    if( hasComponent(AbstractScaleDraw::Labels) )
    {
        drawLabels(painter, indexPos, indexLabel, palette);
    }

    if ( hasComponent( AbstractScaleDraw::Ticks ) )
    {
        double tickLen = tickLength(QwtScaleDiv::MajorTick);
        drawTicks(painter, indexPos, tickLen, palette);
    }

    if ( hasComponent( AbstractScaleDraw::Backbone ) )
    {
        painter->save();

        QPen pen = painter->pen();
        pen.setColor( palette.color( QPalette::WindowText ) );
        pen.setCapStyle( Qt::FlatCap );

        painter->setPen( pen );

        drawBackbone( painter );

        painter->restore();
    }

    painter->restore();
}

void ItemScaleDraw::drawLabels(QPainter* painter, const QVector<int>& indexPos,
                               const QVector<QString>& indexLabel, const QPalette &palette) const
{
    painter->save();
    painter->setPen( palette.color( QPalette::Text ) ); // ignore pen style

    QRectF lastRect = QRectF(DBL_MIN, DBL_MIN, 0, 0);

    for(int i = 0; i < indexPos.size(); i++){
        QwtText lbl = indexLabel[i];
        if(lbl.isEmpty()){
            continue;
        }

        lbl.setRenderFlags( 0 );
        lbl.setLayoutAttribute( QwtText::MinimumLayout );

        QPointF labelPos = itemLabelPosition( indexPos[i] );
        QSizeF labelSize = lbl.textSize( painter->font() );
        QRectF currentRect = QRectF(labelPos, labelSize);

        if(lastRect.intersects(currentRect))
            continue;

        lastRect = currentRect;

        const QTransform transform = labelTransformation( labelPos, labelSize );

        painter->save();
        painter->setWorldTransform( transform, true );
        lbl.draw ( painter, QRect( QPoint( 0, 0 ), labelSize.toSize() ) );
        painter->restore();
    }
    painter->restore();
}

void ItemScaleDraw::drawTicks(QPainter *painter, const QVector<int> &indexPos, double len, const QPalette &palette) const
{
    if ( len <= 0 ) return;

    painter->save();

    QPen pen = painter->pen();
    pen.setColor( palette.color( QPalette::WindowText ) );
    pen.setCapStyle( Qt::FlatCap );

    painter->setPen( pen );

    const bool roundingAlignment = QwtPainter::roundingAlignment( painter );
    const int pw = penWidth();
    int a = (pw > 1 && roundingAlignment) ? 1 : 0;

    for(int i = 0; i < indexPos.size(); i++){
        QPointF pos = getPos();
        double tval = indexPos[i];

        if(roundingAlignment)
            tval = qRound(tval);

        switch ( getAlignment() )
        {
            case AbstractAxis::LeftScale:
            {
                double x1 = pos.x() + a;
                double x2 = pos.x() + a - pw - len;
                if ( roundingAlignment )
                {
                    x1 = qRound( x1 );
                    x2 = qRound( x2 );
                }

                QwtPainter::drawLine( painter, x1, tval, x2, tval );
                break;
            }

            case AbstractAxis::RightScale:
            {
                double x1 = pos.x();
                double x2 = pos.x() + pw + len;
                if ( roundingAlignment )
                {
                    x1 = qRound( x1 );
                    x2 = qRound( x2 );
                }

                QwtPainter::drawLine( painter, x1, tval, x2, tval );
                break;
            }

            case AbstractAxis::BottomScale:
            {
                double y1 = pos.y();
                double y2 = pos.y() + pw + len;
                if ( roundingAlignment )
                {
                    y1 = qRound( y1 );
                    y2 = qRound( y2 );
                }

                QwtPainter::drawLine( painter, tval, y1, tval, y2 );
                break;
            }

            case AbstractAxis::TopScale:
            {
                double y1 = pos.y() + a;
                double y2 = pos.y() - pw - len + a;
                if ( roundingAlignment )
                {
                    y1 = qRound( y1 );
                    y2 = qRound( y2 );
                }

                QwtPainter::drawLine( painter, tval, y1, tval, y2 );
                break;
            }
        }
    }
    painter->restore();
}

int ItemScaleDraw::maxLabelHeight(const QFont &font) const
{
    double maxHeight = 0.0;

    const QMap<int, QString>& indexMap = itemAxis->getMajorTickSeries();
    for(QMap<int, QString>::const_iterator i = indexMap.begin(); i != indexMap.end(); i++){
        // indexMap -
        const double h = labelSize( font, i.value() ).height();
        if ( h > maxHeight )
            maxHeight = h;
    }

    return qCeil( maxHeight );
}

int ItemScaleDraw::maxLabelWidth(const QFont &font) const
{
    double maxWidth = 0.0;

    const QMap<int, QString>& indexMap = itemAxis->getMajorTickSeries();
    for(QMap<int, QString>::const_iterator i = indexMap.begin(); i != indexMap.end(); i++){
        // indexMap -
        const double h = labelSize( font, i.value() ).width();
        if ( h > maxWidth )
            maxWidth = h;
    }

    return qCeil( maxWidth );
}

QPointF ItemScaleDraw::itemLabelPosition(int pos) const
{
    // 
    double tval = pos;
    double dist = spacing();

    if ( hasComponent( AbstractScaleDraw::Backbone ) )
        dist += qMax( 1, penWidth() );

    if ( hasComponent( AbstractScaleDraw::Ticks ) )
        dist += tickLength( QwtScaleDiv::MajorTick );

    double px = 0;
    double py = 0;

    switch ( getAlignment() ){
        case AbstractAxis::RightScale:{
            px = getPos().x() + dist;
            py = tval;
            break;
        }
        case AbstractAxis::LeftScale:{
            px = getPos().x() - dist;
            py = tval;
            break;
        }
        case AbstractAxis::BottomScale:{
            px = tval;
            py = getPos().y() + dist;
            break;
        }
        case AbstractAxis::TopScale:{
            px = tval;
            py = getPos().y() - dist;
            break;
        }
    }

    return QPointF( px, py );
}

}
