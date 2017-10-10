#include "simplescalewidget.h"
#include "qwt_painter.h"
#include "qwt_color_map.h"
#include "qwt_scale_map.h"
#include "qwt_math.h"
#include "qwt_scale_div.h"
#include "qwt_text.h"
#include "qwt_scale_engine.h"
#include "qwt_transform.h"
#include "rectangularaxisdraw.h"
#include <QPainter>
#include <QEvent>
#include <QtMath>
#include <QStyle>
#include <QStyleOption>
#include <assert.h>

class SimpleScaleWidget::PrivateData
{
public:
    PrivateData():
        scaleDraw( NULL )
    {
    }

    ~PrivateData()
    {
        delete scaleDraw;
    }

    RectangularAxisDraw *scaleDraw;

    int borderDist[2];
    int minBorderDist[2];
    int scaleLength;
    int margin;

    int titleOffset;
    int spacing;
    QwtText title;

    SimpleScaleWidget::LayoutFlags layoutFlags;
};

/*!
  \brief Create a scale with the position SimpleScaleWidget::Left
  \param parent Parent widget
*/
SimpleScaleWidget::SimpleScaleWidget( QWidget *parent ):
    AbstractTraceAxis( parent )
{
    initScale( AbstractAxis::LeftScale );
}

/*!
  \brief Constructor
  \param align Alignment.
  \param parent Parent widget
*/
SimpleScaleWidget::SimpleScaleWidget(AxisType align, QWidget *parent ):
    AbstractTraceAxis( align, parent )
{
    initScale( align );
}

//! Destructor
SimpleScaleWidget::~SimpleScaleWidget()
{
    delete d_data;
}

AbstractPlot::Axis SimpleScaleWidget::getAxisIndex() const
{
    switch(getType()){
    case AxisType::TopScale: return AbstractPlot::xTop;
    case AxisType::BottomScale: return AbstractPlot::xBottom;
    case AxisType::LeftScale: return AbstractPlot::yLeft;
    case AxisType::RightScale: return AbstractPlot::yRight;
    default:
        assert(false); // 出现了异常的坐标轴类型。
        return AbstractPlot::axisCnt;
    }
}

//! Initialize the scale
void SimpleScaleWidget::initScale(AxisType align )
{
    d_data = new PrivateData;

    d_data->layoutFlags = 0;
    if ( align == AbstractAxis::RightScale )
        d_data->layoutFlags |= TitleInverted;

    d_data->minBorderDist[0] = 0;
    d_data->minBorderDist[1] = 0;
    d_data->margin = 4;
    d_data->titleOffset = 0;
    d_data->spacing = 2;

    d_data->scaleDraw = new RectangularAxisDraw;
    d_data->scaleDraw->setAlignment( align );
    d_data->scaleDraw->setLength( 10 );

    setScaleEngine(new QwtLinearScaleEngine());

    d_data->scaleDraw->setScaleDiv(
        getScaleEngine()->divideScale( 0.0, 100.0, 10, 5 ) );

    setCanvasValueRange(0.0, 100.0, 0.0);

    const int flags = Qt::AlignHCenter
        | Qt::TextExpandTabs | Qt::TextWordWrap;
    d_data->title.setRenderFlags( flags );
    d_data->title.setFont( font() );

    QSizePolicy policy( QSizePolicy::MinimumExpanding,
        QSizePolicy::Fixed );
    if ( d_data->scaleDraw->orientation() == Qt::Vertical )
        policy.transpose();

    setSizePolicy( policy );

    setAttribute( Qt::WA_WState_OwnSizePolicy, false );
}

/*!
   Toggle an layout flag

   \param flag Layout flag
   \param on true/false

   \sa testLayoutFlag(), LayoutFlag
*/
void SimpleScaleWidget::setLayoutFlag( LayoutFlag flag, bool on )
{
    if ( ( ( d_data->layoutFlags & flag ) != 0 ) != on )
    {
        if ( on )
            d_data->layoutFlags |= flag;
        else
            d_data->layoutFlags &= ~flag;
    }
}

/*!
   Test a layout flag

   \param flag Layout flag
   \return true/false
   \sa setLayoutFlag(), LayoutFlag
*/
bool SimpleScaleWidget::testLayoutFlag( LayoutFlag flag ) const
{
    return ( d_data->layoutFlags & flag );
}


/*!
  Change the alignment

  \param alignment New alignment
  \sa alignment()
*/
void SimpleScaleWidget::setAlignment( AbstractAxis::AxisType alignment )
{
    if ( d_data->scaleDraw )
        d_data->scaleDraw->setAlignment( alignment );

    if ( !testAttribute( Qt::WA_WState_OwnSizePolicy ) )
    {
        QSizePolicy policy( QSizePolicy::MinimumExpanding,
            QSizePolicy::Fixed );
        if ( d_data->scaleDraw->orientation() == Qt::Vertical )
            policy.transpose();

        setSizePolicy( policy );

        setAttribute( Qt::WA_WState_OwnSizePolicy, false );
    }

    updateLayout();
}


/*!
    \return position
    \sa setPosition()
*/
AbstractAxis::AxisType SimpleScaleWidget::alignment() const
{
    if ( !scaleDraw() )
        return AbstractAxis::LeftScale;

    return scaleDraw()->getAlignment();
}

/*!
  \brief Specify the distance between color bar, scale and title
  \param spacing Spacing
  \sa spacing()
*/
void SimpleScaleWidget::setSpacing( int spacing )
{
    spacing = qMax( 0, spacing );
    if ( spacing != d_data->spacing )
    {
        d_data->spacing = spacing;
        updateLayout();
    }
}

/*!
  \brief Change the alignment for the labels.

  \sa RectangularAxisDraw::setLabelAlignment(), setLabelRotation()
*/
void SimpleScaleWidget::setLabelAlignment( Qt::Alignment alignment )
{
    d_data->scaleDraw->setLabelAlignment( alignment );
    updateLayout();
}

/*!
  \brief Change the rotation for the labels.
  See RectangularAxisDraw::setLabelRotation().

  \param rotation Rotation
  \sa RectangularAxisDraw::setLabelRotation(), setLabelFlags()
*/
void SimpleScaleWidget::setLabelRotation( double rotation )
{
    d_data->scaleDraw->setLabelRotation( rotation );
    updateLayout();
}

/*!
  Set a scale draw

  scaleDraw has to be created with new and will be deleted in
  ~SimpleScaleWidget() or the next call of setScaleDraw().
  scaleDraw will be initialized with the attributes of
  the previous scaleDraw object.

  \param scaleDraw ScaleDraw object
  \sa scaleDraw()
*/
void SimpleScaleWidget::setScaleDraw( RectangularAxisDraw *scaleDraw )
{
    if ( ( scaleDraw == NULL ) || ( scaleDraw == d_data->scaleDraw ) )
        return;

    const RectangularAxisDraw* sd = d_data->scaleDraw;
    if ( sd )
    {
        scaleDraw->setAlignment( sd->getAlignment() );
        scaleDraw->setScaleDiv( sd->scaleDiv() );

        QwtTransform *transform = NULL;
        if ( sd->scaleMap().transformation() )
            transform = sd->scaleMap().transformation()->copy();

        scaleDraw->setTransformation( transform );
    }

    delete d_data->scaleDraw;
    d_data->scaleDraw = scaleDraw;

    //! 在构造函数中使用这个函数设置scaleDraw会调用updateLayout()这个虚函数，这会导致一些异常结果。
    //! \todo 使用更好的方法解决问题。
    //! updateLayout();
}

/*!
    \return scaleDraw of this scale
    \sa setScaleDraw(), RectangularAxisDraw::setScaleDraw()
*/
const RectangularAxisDraw *SimpleScaleWidget::scaleDraw() const
{
    return d_data->scaleDraw;
}

/*!
    \return scaleDraw of this scale
    \sa RectangularAxisDraw::setScaleDraw()
*/
RectangularAxisDraw *SimpleScaleWidget::scaleDraw()
{
    return d_data->scaleDraw;
}

QwtScaleMap SimpleScaleWidget::caculateCanvasMap(const QWidget *canvas) const
{
    QwtScaleMap map;
    if ( !canvas )
        return map;

    map.setTransformation( scaleEngine->transformation() );

    // QwtScaleDiv的lowerBound和upperBound是坐标轴的数值范围，即backbone区域的表示的数值范围
    const QwtScaleDiv &sd = scaleDraw()->scaleDiv();
    map.setScaleInterval( getCanvasMinValue(), getCanvasMaxValue() );

    if ( isAxisEnabled() ){
        // 计算坐标轴backbone区域的本地坐标
        //! \warning scaleMap()必须有效。如果scaleMap的PaintInterval为0，则变换结果总是p1 = p2。
        double p1 = scaleDraw()->scaleMap().transform(sd.lowerBound());
        double p2 = scaleDraw()->scaleMap().transform(sd.upperBound());
        // 坐标轴方向上，本地坐标到canvas坐标的偏移差值。
        int offset = 0;

        if ( scaleDraw()->orientation() == Qt::Vertical ){
            offset = canvas->mapFromGlobal(mapToGlobal(QPoint(0, 0))).y();
        }else{
            offset = canvas->mapFromGlobal(mapToGlobal(QPoint(0, 0))).x();
        }
        map.setPaintInterval(p1 + offset, p2 + offset);
    }

    return map;
}

/*!
  \brief Specify the margin to the base line.
  \param margin Margin
  \sa getMargin()
*/
void SimpleScaleWidget::setMargin(int margin)
{
    margin = qMax( 0, margin );
    if ( margin != d_data->margin )
    {
        d_data->margin = margin;
        updateLayout();
    }
}

/*!
    \return margin
    \sa setMargin()
*/
int SimpleScaleWidget::getMargin() const {
    return d_data->margin;
}

/*!
    \return distance between scale and title
    \sa setMargin()
*/
int SimpleScaleWidget::spacing() const
{
    return d_data->spacing;
}

void SimpleScaleWidget::setMaxMajor(int maxMajor){
    maxMajor = qBound( 1, maxMajor, 10000 );
    this->maxMajor = maxMajor;
}

int SimpleScaleWidget::getMaxMajor() const {
    return maxMajor;
}

void SimpleScaleWidget::setAxisMaxMinor(int maxMinor){
    maxMinor = qBound( 0, maxMinor, 100 );
    this->maxMinor = maxMinor;
}

int SimpleScaleWidget::getMaxMinor() const {
    return maxMinor;
}

/*!
  \brief draw the scale
*/
void SimpleScaleWidget::drawAxis( QPainter *painter ) const
{
    d_data->scaleDraw->draw( painter, palette() );

    QRect r = contentsRect();
    if (d_data->scaleDraw->orientation() == Qt::Horizontal){
        r.setLeft(r.left() + d_data->borderDist[0]);
        r.setWidth(r.width() - d_data->borderDist[1]);
    }else{
        r.setTop(r.top() + d_data->borderDist[0]);
        r.setHeight(r.height() - d_data->borderDist[1]);
    }
}

/*!
 * \brief 基于当前的坐标轴的几何大小和字体重新计算坐标轴的几何大小和布局。
 * \param update_geometry 通知布局系统并且调用update()来重绘坐标轴。
 */
void SimpleScaleWidget::updateLayout( bool update_geometry ){
    //! 首先根据当前BorderDistPolicy重新计算最合适的BorderDist
    //! 更新布局时，只有这一次根据自身需要调整BorderDist的机会，在其他场合对于BorderDist都是直接获取。
    adjustBorderDist(false);

    int bd0 = getStartBorderDist();
    int bd1 = getEndBorderDist();

    const QRectF r = contentsRect();
    double x, y, length;

    if ( d_data->scaleDraw->orientation() == Qt::Vertical ){
        y = r.top() + bd0;
        length = r.height() - ( bd0 + bd1 );

        if ( d_data->scaleDraw->getAlignment() == AbstractAxis::LeftScale )
            x = r.right() - 1.0 - d_data->margin;
        else
            x = r.left() + d_data->margin;
    }else{
        x = r.left() + bd0;
        length = r.width() - ( bd0 + bd1 );

        if ( d_data->scaleDraw->getAlignment() == AbstractAxis::BottomScale )
            y = r.top() + d_data->margin;
        else
            y = r.bottom() - 1.0 - d_data->margin;
    }

    // 设置坐标轴线位置和长度。
    d_data->scaleDraw->move( x, y );
    d_data->scaleDraw->setLength( length );

    const int extent = qCeil( d_data->scaleDraw->extent( font() ) );

    d_data->titleOffset =
        d_data->margin + d_data->spacing + extent;

    if ( update_geometry ){
        updateGeometry();
        update();
    }
}

/*!
  \brief Notify a change of the scale

  This virtual function can be overloaded by derived
  classes. The default implementation updates the geometry
  and repaints the widget.
*/

void SimpleScaleWidget::scaleChange(){
    updateLayout();
}

/*!
  \return a size hint
*/
QSize SimpleScaleWidget::sizeHint() const{
    return minimumSizeHint();
}
/*!
  \return a minimum size hint
*/
QSize SimpleScaleWidget::minimumSizeHint() const
{
    const Qt::Orientation o = d_data->scaleDraw->orientation();

    // Border Distance cannot be less than the scale borderDistHint
    // Note, the borderDistHint is already included in minHeight/minWidth
    int length = 0;
    int mbd1, mbd2;
    getBorderDistHint( mbd1, mbd2 );
    length += qMax( 0, d_data->borderDist[0] - mbd1 );
    length += qMax( 0, d_data->borderDist[1] - mbd2 );
    length += d_data->scaleDraw->minLength( font() );

    int dim = dimForLength( length, font() );

    if ( length < dim )
    {
        // compensate for long titles
        length = dim;
        dim = dimForLength( length, font() );
    }

    QSize size( length + 2, dim );
    if ( o == Qt::Vertical )
        size.transpose();

    int left, right, top, bottom;
    getContentsMargins( &left, &top, &right, &bottom );
    QSize result = size + QSize( left + right, top + bottom );

    /***********************************************************
    //在坐标轴方向上可以无限压缩
    if(o==Qt::Horizontal){
        result.setWidth(0);
    }else{
        result.setHeight(0);
    }
    ***********************************************************/

    return result;
}

/*!
 * \return 返回tick方向（坐标轴垂直方向）上最合适的宽度
 */
int SimpleScaleWidget::dimHint() const{
    switch(alignment()){
    case AbstractAxis::LeftScale:
    case AbstractAxis::RightScale:
        return sizeHint().width();
        //return 56;
    case AbstractAxis::TopScale:
    case AbstractAxis::BottomScale:
        return sizeHint().height();
    default:
        return 0;
    }
}

/*!
  \brief Find the height of the title for a given width.
  \param width Width
  \return height Height
 */

int SimpleScaleWidget::titleHeightForWidth( int width ) const
{
    return qCeil( d_data->title.heightForWidth( width, font() ) );
}

/*!
  \brief Find the minimum dimension for a given length.
         dim is the height, length the width seen in
         direction of the title.
  \param length width for horizontal, height for vertical scales
  \param scaleFont Font of the scale
  \return height for horizontal, width for vertical scales
*/

int SimpleScaleWidget::dimForLength( int length, const QFont &scaleFont ) const
{
    const int extent = qCeil( d_data->scaleDraw->extent( scaleFont ) );

    int dim = d_data->margin + extent + 1;

    if ( !d_data->title.isEmpty() )
        dim += titleHeightForWidth( length ) + d_data->spacing;

    return dim;
}

/*!
 * \brief 计算布局时期望使用的边距（border distances）。
 * \param start start border distance。
 * \param end end border distance。
 * \warning <ul> <li>最小边距取决于使用的字体。</ul>
 * \sa setMinBorderDist(), getMinBorderDist(), setBorderDist()
 */
void SimpleScaleWidget::getBorderDistHint( int &start, int &end ) const {
    d_data->scaleDraw->getBorderDistHint( font(), start, end );

    start = qMax(start, d_data->minBorderDist[0]);
    end = qMax(end,d_data->minBorderDist[1]);
}

void SimpleScaleWidget::setCanvasValueRange(double min, double max, double stepSize){
    minValue = min;
    maxValue = max;
    this->stepSize = stepSize;
}

void SimpleScaleWidget::updateScaleDiv(){
    double minValue,maxValue;
    getCanvasValueRange(minValue, maxValue);
    double stepSize = getStepSize();

    setScaleDiv(getScaleEngine()->divideScale(
        minValue, maxValue, getMaxMajor(), getMaxMinor(), stepSize ));

    adjustBorderDist();
}

double SimpleScaleWidget::getCanvasMaxValue() const { return maxValue; }
double SimpleScaleWidget::getCanvasMinValue() const { return minValue; }

void SimpleScaleWidget::getCanvasValueRange(double &min, double &max) const{
    min = minValue;
    max = maxValue;
}

/*!
  \brief Assign a scale division

  The scale division determines where to set the tick marks.

  \param scaleDiv Scale Division
  \sa For more information about scale divisions, see QwtScaleDiv.
*/
void SimpleScaleWidget::setScaleDiv( const QwtScaleDiv &scaleDiv )
{
    RectangularAxisDraw *sd = d_data->scaleDraw;
    if ( sd->scaleDiv() != scaleDiv )
    {
        sd->setScaleDiv( scaleDiv );

        emit scaleDivChanged();
    }
}

void SimpleScaleWidget::setScaleEngine(QwtScaleEngine *engine){
    if(scaleEngine != nullptr) delete scaleEngine;
    scaleEngine = engine;
}

const QwtScaleEngine *SimpleScaleWidget::getScaleEngine() const {
    return const_cast<const QwtScaleEngine*>(scaleEngine);
}

QwtScaleEngine *SimpleScaleWidget::getScaleEngine(){
    return scaleEngine;
}

/*!
  Set the transformation

  \param transformation Transformation
  \sa QwtAbstractScaleDraw::scaleDraw(), QwtScaleMap
 */
void SimpleScaleWidget::setTransformation( QwtTransform *transformation )
{
    d_data->scaleDraw->setTransformation( transformation );
    updateLayout();
}

