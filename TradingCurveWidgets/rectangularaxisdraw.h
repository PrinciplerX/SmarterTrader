#ifndef RECTANGULARAXISDRAW_H
#define RECTANGULARAXISDRAW_H

#include "tradingcurvewidgets_global.h"
#include "abstractscaledraw.h"
#include <QPoint>
#include <QRect>
#include <QTransform>
#include "abstractaxis.h"
#include "qwt_scale_div.h"
#include "qwt_scale_map.h"
#include "qwt_painter.h"

/*!
 * \brief ֱ��������Ļ��ƹ��ߡ�
 *
 * ֱ��������ָ����һ��ֱ������������ߣ��̶��Լ��̶Ƚű�������ᡣ
 *
 * RectangularAxisDraw can be used to draw linear or logarithmic scales.
 * A scale has a position, an alignment and a length, which can be specified .
 * The labels can be rotated and aligned
 * to the ticks using setLabelRotation() and setLabelAlignment().
 *
 * After a scale division has been specified as a QwtScaleDiv object
 * using QwtAbstractScaleDraw::setScaleDiv(const QwtScaleDiv &s),
 * the scale can be drawn with the QwtAbstractScaleDraw::draw() member.
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT RectangularAxisDraw : public AbstractScaleDraw
{
public:
    RectangularAxisDraw();

    AbstractAxis::AxisType getAlignment() const;
    void setAlignment( AbstractAxis::AxisType );

    void move( double x, double y );
    void move( const QPointF & );
    void setLength( double length );
    QPointF getPos() const;
    double length() const;

    Qt::Orientation orientation() const;

    void setLabelAlignment( Qt::Alignment );
    Qt::Alignment getLabelAlignment() const;

    void setLabelRotation( double rotation );
    double getLabelRotation() const;

    void getBorderDistHint( const QFont &, int &start, int &end ) const;
    int minLength( const QFont & ) const;

    int minLabelDist( const QFont & ) const;

    virtual double extent( const QFont & ) const;

    /*!
     * \return ��ʾ�����׼����ֵ
     */
    virtual QwtText label( double value ) const override;

    virtual int maxLabelHeight( const QFont & ) const;
    virtual int maxLabelWidth( const QFont & ) const;

    QPointF labelPosition( double val ) const;

    QRectF labelRect( const QFont &, double val ) const;
    QSizeF labelSize( const QFont &, double val ) const;
    QSizeF labelSize( const QFont &, const QString& label) const;

    QRect boundingLabelRect( const QFont &, double val ) const;

    QTransform labelTransformation( const QPointF &, const QSizeF & ) const;

    virtual void drawTick( QPainter *, double val, double len ) const override;
    virtual void drawBackbone( QPainter * ) const override;
    virtual void drawLabel( QPainter *, double val ) const override;

    virtual void updateMap();

private:
    //backbone����㣨backbone�����˵��У�x��y�ܺͽ�С���Ǹ���
    QPointF pos;

    //backbone�ĳ���
    double len;

    AbstractAxis::AxisType alignment;

    Qt::Alignment labelAlignment;
    double labelRotation;
};

/*!
 * \brief �ƶ��������λ�á�
 * \param x �����ꡣ
 * \param y �����ꡣ
 * \sa move(const QPointF &)
 */
inline void RectangularAxisDraw::move(double x, double y){
    move( QPointF( x, y ) );
}

#endif // RECTANGULARAXISDRAW_H
