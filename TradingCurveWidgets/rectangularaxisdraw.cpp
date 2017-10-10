#include "rectangularaxisdraw.h"
#include <QtMath>
#include "qwt_math.h"
/*!
  \brief Constructor

  The range of the scale is initialized to [0, 100],
  The position is at (0, 0) with a length of 100.
  The orientation is AbstractScaleDraw::Bottom.
*/
RectangularAxisDraw::RectangularAxisDraw():
    len( 0 ),
    alignment( AbstractAxis::BottomScale ),
    labelAlignment( 0 ),
    labelRotation( 0.0 )
{
    setLength( 100 );
}

/*!
   Return alignment of the scale
   \sa setAlignment()
   \return Alignment of the scale
*/
AbstractAxis::AxisType RectangularAxisDraw::getAlignment() const
{
    return alignment;
}

/*!
   Set the alignment of the scale

   \param align Alignment of the scale

   The default alignment is AbstractAxis::BottomScale
   \sa alignment()
*/
void RectangularAxisDraw::setAlignment( AbstractAxis::AxisType align )
{
    alignment = align;
}


/*!
  \brief Move the position of the scale

  The meaning of the parameter pos depends on the alignment:
  <dl>
  <dt>AbstractAxis::LeftScale
  <dd>The origin is the topmost point of the
      backbone. The backbone is a vertical line.
      Scale marks and labels are drawn
      at the left of the backbone.
  <dt>AbstractAxis::RightScale
  <dd>The origin is the topmost point of the
      backbone. The backbone is a vertical line.
      Scale marks and labels are drawn
      at the right of the backbone.
  <dt>AbstractAxis::TopScale
  <dd>The origin is the leftmost point of the
      backbone. The backbone is a horizontal line.
      Scale marks and labels are drawn
      above the backbone.
  <dt>AbstractAxis::BottomScale
  <dd>The origin is the leftmost point of the
      backbone. The backbone is a horizontal line
      Scale marks and labels are drawn
      below the backbone.
  </dl>

  \param pos Origin of the scale

  \sa pos(), setLength()
*/
void RectangularAxisDraw::move( const QPointF &pos )
{
    this->pos = pos;
    updateMap();
}

/*!
  Set the length of the backbone.

  The length doesn't include the space needed for
  overlapping labels.

  \param length Length of the backbone

  \sa move(), minLabelDist()
*/
void RectangularAxisDraw::setLength( double length )
{
    len = qMax(length, 0.0);
    updateMap();
}

/*!
   \return Origin of the scale
   \sa move(), length()
*/
QPointF RectangularAxisDraw::getPos() const {
    return pos;
}

/*!
   \return the length of the backbone
   \sa setLength(), pos()
*/
double RectangularAxisDraw::length() const {
    return len;
}

/*!
  Return the orientation

  TopScale, BottomScale are horizontal (Qt::Horizontal) scales,
  LeftScale, RightScale are vertical (Qt::Vertical) scales.

  \return Orientation of the scale

  \sa alignment()
*/
Qt::Orientation RectangularAxisDraw::orientation() const
{
    switch ( alignment )
    {
        case AbstractAxis::TopScale:
        case AbstractAxis::BottomScale:
            return Qt::Horizontal;
        case AbstractAxis::LeftScale:
        case AbstractAxis::RightScale:
        default:
            return Qt::Vertical;
    }
}

/*!
  Rotate all labels.

  When changing the rotation, it might be necessary to
  adjust the label flags too. Finding a useful combination is
  often the result of try and error.

  \param rotation Angle in degrees. When changing the label rotation,
                  the label flags often needs to be adjusted too.

  \sa setLabelAlignment(), labelRotation(), labelAlignment().

*/
void RectangularAxisDraw::setLabelRotation( double rotation )
{
    labelRotation = rotation;
}

/*!
  \return the label rotation
  \sa setLabelRotation(), labelAlignment()
*/
double RectangularAxisDraw::getLabelRotation() const
{
    return labelRotation;
}

/*!
 * \brief 计算坐标轴刻度脚标的限定矩形（bounding rectangle）。该矩形的坐标是相对于在刻度（tick）方向上，到坐标
 * 轴线（backbone）spacing + 刻度值长度的位置。
 *
 * |
 * +-- 100.0
 * |
 * +-- 90.0
 * |
 *
 * \param font 用于绘制的字体。
 * \param value 数值。
 * \return 用来绘制刻度值脚标的限定矩形。
 */
QRectF RectangularAxisDraw::labelRect( const QFont &font, double value ) const
{
    QwtText lbl = tickLabel( font, value );
    if ( lbl.isEmpty() )
        return QRectF( 0.0, 0.0, 0.0, 0.0 );

    const QPointF pos = labelPosition( value );

    const QSizeF labelSize = lbl.textSize( font );
    const QTransform transform = labelTransformation( pos, labelSize );

    QRectF br = transform.mapRect( QRectF( QPointF( 0, 0 ), labelSize ) );
    br.translate( -pos.x(), -pos.y() );

    return br;
}

/*!
  \brief Change the label flags

  Labels are aligned to the point tick length + spacing away from the backbone.

  The alignment is relative to the orientation of the label text.
  In case of an flags of 0 the label will be aligned
  depending on the orientation of the scale:

      AbstractAxis::TopScale: Qt::AlignHCenter | Qt::AlignTop\n
      AbstractAxis::BottomScale: Qt::AlignHCenter | Qt::AlignBottom\n
      AbstractAxis::LeftScale: Qt::AlignLeft | Qt::AlignVCenter\n
      AbstractAxis::RightScale: Qt::AlignRight | Qt::AlignVCenter\n

  Changing the alignment is often necessary for rotated labels.

  \param alignment Or'd Qt::AlignmentFlags see <qnamespace.h>

  \sa setLabelRotation(), labelRotation(), labelAlignment()
  \warning The various alignments might be confusing.
           The alignment of the label is not the alignment
           of the scale and is not the alignment of the flags
           ( QwtText::flags() ) returned from AbstractScaleDraw::label().
*/

void RectangularAxisDraw::setLabelAlignment( Qt::Alignment alignment )
{
    labelAlignment = alignment;
}

/*!
  \return the label flags
  \sa setLabelAlignment(), labelRotation()
*/
Qt::Alignment RectangularAxisDraw::getLabelAlignment() const
{
    return labelAlignment;
}

/*!
 * \brief 计算最小边距（border distance）。
 * \param font 坐标轴脚标（label）使用的字体。
 * \param start 前端边距（start border distance），坐标轴最上或最左位置上的刻度。
 * \param end 后端边距（start border distance），坐标轴最下或最右位置上的刻度。
 */
void RectangularAxisDraw::getBorderDistHint( const QFont &font, int &start, int &end ) const
{
    start = 0;
    end = 1.0;

    if ( !hasComponent( AbstractScaleDraw::Labels ) )
        return;

    // label只会在MajorTick上标出,所以只需要考虑MajorTick
    const QList<double> &ticks = scaleDiv().ticks( QwtScaleDiv::MajorTick );
    if ( ticks.count() == 0 )
        return;

    // 寻找映射到边界（borders）的刻度（ticks）。
    // minTick 是映射到控件坐标系中最上、最左位置上的刻度。
    // manTick 是映射到控件坐标系中最下、最右位置上的刻度。

    double minTick = ticks[0]; // 坐标轴方向上坐标最小的tick的数值
    double minPos = scaleMap().transform( minTick );
    double maxTick = minTick; // 坐标轴方向上坐标最大的tick的数值
    double maxPos = minPos;

    // 找出位于坐标轴方向上离边界最近的两个刻度值和刻度位置。
    for ( int i = 1; i < ticks.count(); i++ ){
        const double tickPos = scaleMap().transform( ticks[i] );

        if ( tickPos < minPos ){
            minTick = ticks[i];
            minPos = tickPos;
        }

        if ( tickPos > maxPos ){
            maxTick = ticks[i];
            maxPos = tickPos;
        }
    }

    // e 是映射到控件坐标系中最上、最左位置上的边距。
    // s 是映射到控件坐标系中最下、最右位置上的边距。
    double e = 0.0;
    double s = 0.0;

    if ( orientation() == Qt::Vertical ){
        s = -labelRect( font, minTick ).top();
        //s -= qAbs( minPos - qRound( scaleMap().p2() ) );

        e = labelRect( font, maxTick ).bottom();
        //e -= qAbs( maxPos - scaleMap().p1() );
    }else{
        s = -labelRect( font, minTick ).left();
        //s -= qAbs( minPos - scaleMap().p1() );

        e = labelRect( font, maxTick ).right();
        //e -= qAbs( maxPos - scaleMap().p2() );
    }

    s = qMax(s, 0.0);
    e = qMax(e, 0.0);

    start = qCeil( s );
    end = qCeil( e );
}


/*!
 * \brief 计算要绘制坐标轴所需要的最小长度（坐标轴方向上的长度），以使坐标轴上的label
 * 彼此不重合。
 *
 * \param font Font used for painting the labels
 * \return Minimum length that is needed to draw the scale
 *
 * \sa extent()
 */

int RectangularAxisDraw::minLength( const QFont &font ) const
{
    int startDist, endDist;
    getBorderDistHint( font, startDist, endDist );

    const QwtScaleDiv &sd = scaleDiv();

    const uint minorCount =
        sd.ticks( QwtScaleDiv::MinorTick ).count() +
        sd.ticks( QwtScaleDiv::MediumTick ).count();
    const uint majorCount =
        sd.ticks( QwtScaleDiv::MajorTick ).count();

    int lengthForLabels = 0;
    if ( hasComponent( AbstractScaleDraw::Labels ) )
        lengthForLabels = minLabelDist( font ) * majorCount;

    int lengthForTicks = 0;
    if ( hasComponent( AbstractScaleDraw::Ticks ) )
    {
        const double pw = qMax( 1, penWidth() );  // penwidth can be zero
        lengthForTicks = qCeil( ( majorCount + minorCount ) * ( pw + 1.0 ) );
    }

    return startDist + endDist + qMax( lengthForLabels, lengthForTicks );
}

void RectangularAxisDraw::updateMap()
{
    const QPointF pos = this->pos;
    double len = this->len;

    QwtScaleMap &sm = scaleMap();
    if ( orientation() == Qt::Vertical )
        sm.setPaintInterval( pos.y() + len, pos.y() );
    else
        sm.setPaintInterval( pos.x() , pos.x() + len );
}

#if QT_VERSION < 0x040601
#define qFastSin(x) qSin(x)
#define qFastCos(x) qCos(x)
#endif

/*!
  Determine the minimum distance between two labels, that is necessary
  that the texts don't overlap.

  \param font Font
  \return The maximum width of a label

  \sa getBorderDistHint()
*/

int RectangularAxisDraw::minLabelDist( const QFont &font ) const
{
    if ( !hasComponent( AbstractScaleDraw::Labels ) )
        return 0;

    const QList<double> &ticks = scaleDiv().ticks( QwtScaleDiv::MajorTick );
    if ( ticks.isEmpty() )
        return 0;

    const QFontMetrics fm( font );

    const bool vertical = ( orientation() == Qt::Vertical );

    QRectF bRect1;
    QRectF bRect2 = labelRect( font, ticks[0] );
    if ( vertical )
    {
        bRect2.setRect( -bRect2.bottom(), 0.0, bRect2.height(), bRect2.width() );
    }

    double maxDist = 0.0;

    for ( int i = 1; i < ticks.count(); i++ )
    {
        bRect1 = bRect2;
        bRect2 = labelRect( font, ticks[i] );
        if ( vertical )
        {
            bRect2.setRect( -bRect2.bottom(), 0.0,
                bRect2.height(), bRect2.width() );
        }

        double dist = fm.leading(); // space between the labels
        if ( bRect1.right() > 0 )
            dist += bRect1.right();
        if ( bRect2.left() < 0 )
            dist += -bRect2.left();

        if ( dist > maxDist )
            maxDist = dist;
    }

    double angle = qwtRadians( getLabelRotation() );
    if ( vertical )
        angle += M_PI / 2;

    const double sinA = qFastSin( angle ); // qreal -> double
    if ( qFuzzyCompare( sinA + 1.0, 1.0 ) )
        return qCeil( maxDist );

    const int fmHeight = fm.ascent() - 2;

    // The distance we need until there is
    // the height of the label font. This height is needed
    // for the neighbored label.

    double labelDist = fmHeight / qFastSin( angle ) * qFastCos( angle );
    if ( labelDist < 0 )
        labelDist = -labelDist;

    // For text orientations close to the scale orientation

    if ( labelDist > maxDist )
        labelDist = maxDist;

    // For text orientations close to the opposite of the
    // scale orientation

    if ( labelDist < fmHeight )
        labelDist = fmHeight;

    return qCeil( labelDist );
}

/*!
   Calculate the width/height that is needed for a
   vertical/horizontal scale.

   The extent is calculated from the pen width of the backbone,
   the major tick length, the spacing and the maximum width/height
   of the labels.

   \param font Font used for painting the labels
   \return Extent

   \sa minLength()
*/
double RectangularAxisDraw::extent( const QFont &font ) const
{
    double d = 0;

    if ( hasComponent( AbstractScaleDraw::Labels ) )
    {
        if ( orientation() == Qt::Vertical )
            d = maxLabelWidth( font );
        else
            d = maxLabelHeight( font );

        if ( d > 0 )
            d += spacing();
    }

    if ( hasComponent( AbstractScaleDraw::Ticks ) )
    {
        d += maxTickLength();
    }

    if ( hasComponent( AbstractScaleDraw::Backbone ) )
    {
        const double pw = qMax( 1, penWidth() );  // pen width can be zero
        d += pw;
    }

    d = qMax( d, minimumExtent() );
    return d;
}

/*!
   Find the position, where to paint a label

   The position has a distance that depends on the length of the ticks
   in direction of the getAlignment().

   \param value Value
   \return Position, where to paint a label
*/
QPointF RectangularAxisDraw::labelPosition( double value ) const {
    const double tval = scaleMap().transform( value );

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

void RectangularAxisDraw::drawTick( QPainter *painter, double value, double len ) const
{
    if ( len <= 0 )
        return;

    const bool roundingAlignment = QwtPainter::roundingAlignment( painter );

    QPointF pos = getPos();

    double tval = scaleMap().transform( value );

    const int pw = penWidth();
    int a = 0;
    if ( pw > 1 && roundingAlignment )
        a = 1;

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

/*!
   Draws the baseline of the scale
   \param painter Painter

   \sa drawTick(), drawLabel()
*/
void RectangularAxisDraw::drawBackbone( QPainter *painter ) const
{
    const bool doAlign = QwtPainter::roundingAlignment( painter );

    const QPointF &pos = getPos();
    const double len = this->len;
    const int pw = qMax( penWidth(), 1 );

    // pos indicates a border not the center of the backbone line
    // so we need to shift its position depending on the pen width
    // and the alignment of the scale

    double off;
    if ( doAlign )
    {
        if ( getAlignment() == AbstractAxis::LeftScale || getAlignment() == AbstractAxis::TopScale )
            off = ( pw - 1 ) / 2;
        else
            off = pw / 2;
    }
    else
    {
        off = 0.5 * penWidth();
    }

    switch ( getAlignment() )
    {
        case AbstractAxis::LeftScale:
        {
            double x = pos.x() - off;
            if ( doAlign )
                x = qRound( x );

            QwtPainter::drawLine( painter, x, pos.y(), x, pos.y() + len );
            break;
        }
        case AbstractAxis::RightScale:
        {
            double x = pos.x() + off;
            if ( doAlign )
                x = qRound( x );

            QwtPainter::drawLine( painter, x, pos.y(), x, pos.y() + len );
            break;
        }
        case AbstractAxis::TopScale:
        {
            double y = pos.y() - off;
            if ( doAlign )
                y = qRound( y );

            QwtPainter::drawLine( painter, pos.x(), y, pos.x() + len, y );
            break;
        }
        case AbstractAxis::BottomScale:
        {
            double y = pos.y() + off;
            if ( doAlign )
                y = qRound( y );

            QwtPainter::drawLine( painter, pos.x(), y, pos.x() + len, y );
            break;
        }
    }
}

QwtText RectangularAxisDraw::label( double value ) const {
    //return QLocale().toString(value);
    return QString::number(value, 'f', 2);
}

/*!
   Draws the label for a major scale tick

   \param painter Painter
   \param value Value

   \sa drawTick(), drawBackbone(), boundingLabelRect()
*/
void RectangularAxisDraw::drawLabel( QPainter *painter, double value ) const
{
    QwtText lbl = tickLabel( painter->font(), value );
    if ( lbl.isEmpty() )
        return;

    QPointF pos = labelPosition( value );
    QSizeF labelSize = lbl.textSize( painter->font() );

    const QTransform transform = labelTransformation( pos, labelSize );

    /*! 忽略无法全部显示的脚标。
    QRect mapedRect = transform.mapRect(QRect(QPoint(0,0), labelSize.toSize()));
    if(mapedRect.top() < 0 || mapedRect.bottom() < 0 || mapedRect.left() < 0 || mapedRect.right() < 0){
        return;
    }
    */

    painter->save();

    painter->setWorldTransform( transform, true );

    lbl.draw ( painter, QRect( QPoint( 0, 0 ), labelSize.toSize() ) );

    painter->restore();
}

/*!
  \brief Find the bounding rectangle for the label.

  The coordinates of the rectangle are absolute ( calculated from pos() ).
  in direction of the tick.

  \param font Font used for painting
  \param value Value

  \return Bounding rectangle
  \sa labelRect()
*/
QRect RectangularAxisDraw::boundingLabelRect( const QFont &font, double value ) const
{
    QwtText lbl = tickLabel( font, value );
    if ( lbl.isEmpty() )
        return QRect();

    const QPointF pos = labelPosition( value );
    QSizeF labelSize = lbl.textSize( font );

    const QTransform transform = labelTransformation( pos, labelSize );
    return transform.mapRect( QRect( QPoint( 0, 0 ), labelSize.toSize() ) );
}

/*!
   Calculate the transformation that is needed to paint a label
   depending on its alignment and rotation.

   \param pos Position where to paint the label
   \param size Size of the label

   \return Transformation matrix
   \sa setLabelgetAlignment(), setgetLabelRotation()
*/
QTransform RectangularAxisDraw::labelTransformation(
    const QPointF &pos, const QSizeF &size ) const
{
    QTransform transform;
    transform.translate( pos.x(), pos.y() );
    transform.rotate( getLabelRotation() );

    int flags = getLabelAlignment();
    if ( flags == 0 )
    {
        switch ( getAlignment() )
        {
            case AbstractAxis::RightScale:
            {
                if ( flags == 0 )
                    flags = Qt::AlignRight | Qt::AlignVCenter;
                break;
            }
            case AbstractAxis::LeftScale:
            {
                if ( flags == 0 )
                    flags = Qt::AlignLeft | Qt::AlignVCenter;
                break;
            }
            case AbstractAxis::BottomScale:
            {
                if ( flags == 0 )
                    flags = Qt::AlignHCenter | Qt::AlignBottom;
                break;
            }
            case AbstractAxis::TopScale:
            {
                if ( flags == 0 )
                    flags = Qt::AlignHCenter | Qt::AlignTop;
                break;
            }
        }
    }

    double x, y;

    if ( flags & Qt::AlignLeft )
        x = -size.width();
    else if ( flags & Qt::AlignRight )
        x = 0.0;
    else // Qt::AlignHCenter
        x = -( 0.5 * size.width() );

    if ( flags & Qt::AlignTop )
        y = -size.height();
    else if ( flags & Qt::AlignBottom )
        y = 0;
    else // Qt::AlignVCenter
        y = -( 0.5 * size.height() );

    transform.translate( x, y );

    return transform;
}

/*!
   Calculate the size that is needed to draw a label

   \param font Label font
   \param value Value

   \return Size that is needed to draw a label
*/
QSizeF RectangularAxisDraw::labelSize( const QFont &font, double value ) const
{
    return labelRect( font, value ).size();
}

QSizeF RectangularAxisDraw::labelSize(const QFont &font, const QString &label) const
{
    QwtText lbl(label);
    lbl.setRenderFlags( 0 );
    lbl.setLayoutAttribute( QwtText::MinimumLayout );

    if ( lbl.isEmpty() )
        return QSizeF( 0.0, 0.0 );

    return lbl.textSize( font );
}

/*!
  \param font Font
  \return the maximum width of a label
*/
int RectangularAxisDraw::maxLabelWidth( const QFont &font ) const
{
    double maxWidth = 0.0;

    const QList<double> &ticks = scaleDiv().ticks( QwtScaleDiv::MajorTick );
    for ( int i = 0; i < ticks.count(); i++ )
    {
        const double w = labelSize( font, ticks[i] ).width();
        if ( w > maxWidth )
            maxWidth = w;
    }

    return qCeil( maxWidth );
}

/*!
  \param font Font
  \return the maximum height of a label
*/
int RectangularAxisDraw::maxLabelHeight( const QFont &font ) const
{
    double maxHeight = 0.0;

    const QList<double> &ticks = scaleDiv().ticks( QwtScaleDiv::MajorTick );
    for ( int i = 0; i < ticks.count(); i++ )
    {
        const double v = ticks[i];
        if ( scaleDiv().contains( v ) )
        {
            const double h = labelSize( font, ticks[i] ).height();
            if ( h > maxHeight )
                maxHeight = h;
        }
    }

    return qCeil( maxHeight );
}
