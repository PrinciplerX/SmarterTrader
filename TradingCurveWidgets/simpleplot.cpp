#include "simpleplot.h"
#include "abstractplotlayout.h"
#include "simplescalewidget.h"
#include "qwt_scale_engine.h"
#include "simpleplotcanvas.h"
#include "qwt_text_label.h"
#include <qmath.h>
#include <qpainter.h>
#include <qpointer.h>
#include <qpaintengine.h>
#include <qapplication.h>
#include <qevent.h>

class SimplePlot::AxisData
{
public:
    bool doAutoScale;

    bool isValid;

    SimpleScaleWidget *scaleWidget=NULL;
};

static void qwtSetTabOrder(
    QWidget *first, QWidget *second, bool withChildren )
{
    QList<QWidget *> tabChain;
    tabChain += first;
    tabChain += second;

    if ( withChildren )
    {
        QList<QWidget *> children = second->findChildren<QWidget *>();

        QWidget *w = second->nextInFocusChain();
        while ( children.contains( w ) )
        {
            children.removeAll( w );

            tabChain += w;
            w = w->nextInFocusChain();
        }
    }

    for ( int i = 0; i < tabChain.size() - 1; i++ )
    {
        QWidget *from = tabChain[i];
        QWidget *to = tabChain[i+1];

        const Qt::FocusPolicy policy1 = from->focusPolicy();
        const Qt::FocusPolicy policy2 = to->focusPolicy();

        QWidget *proxy1 = from->focusProxy();
        QWidget *proxy2 = to->focusProxy();

        from->setFocusPolicy( Qt::TabFocus );
        from->setFocusProxy( NULL);

        to->setFocusPolicy( Qt::TabFocus );
        to->setFocusProxy( NULL);

        QWidget::setTabOrder( from, to );

        from->setFocusPolicy( policy1 );
        from->setFocusProxy( proxy1);

        to->setFocusPolicy( policy2 );
        to->setFocusProxy( proxy2 );
    }
}

class SimplePlot::PrivateData
{
public:
    QPointer<QwtTextLabel> titleLabel;
    QPointer<QwtTextLabel> footerLabel;
    //QPointer<QwtAbstractLegend> legend;
    AbstractPlotLayout *layout;

    bool autoReplot;
};

/*!
  \brief Constructor
  \param parent Parent widget
 */
SimplePlot::SimplePlot( QWidget *parent ):
    AbstractPlot( parent )
{
    initPlot( QwtText() );
}

/*!
  \brief Constructor
  \param title Title text
  \param parent Parent widget
 */
SimplePlot::SimplePlot( const QwtText &title, QWidget *parent ):
    AbstractPlot( parent )
{
    initPlot( title );
}

//! Destructor
SimplePlot::~SimplePlot()
{
    enableAutoReplot( false );

    delete d_data->layout;
    deleteAxesData();
    delete d_data;
}

/*!
  \brief Initializes a QwtPlot instance
  \param title Title text
 */
void SimplePlot::initPlot( const QwtText &title )
{
    d_data = new PrivateData;

    d_data->layout = new AbstractPlotLayout;
    d_data->autoReplot = false;

    // title
    d_data->titleLabel = new QwtTextLabel( this );
    d_data->titleLabel->setObjectName( "QwtPlotTitle" );
    d_data->titleLabel->setFont( QFont( fontInfo().family(), 14, QFont::Bold ) );

    QwtText text( title );
    text.setRenderFlags( Qt::AlignCenter | Qt::TextWordWrap );
    d_data->titleLabel->setText( text );

    // footer
    d_data->footerLabel = new QwtTextLabel( this );
    d_data->footerLabel->setObjectName( "QwtPlotFooter" );

    QwtText footer;
    footer.setRenderFlags( Qt::AlignCenter | Qt::TextWordWrap );
    d_data->footerLabel->setText( footer );

    // axis
    initAxesData();

    // canvas
    setCanvas(new SimplePlotCanvas( this ));
    getCanvas()->setObjectName( "SimplePlotCanvas" );
    getCanvas()->installEventFilter( this );

    ///////////////////////////////////////////////////////////////////
    //                                                               //
    //setSizePolicy( QSizePolicy::MinimumExpanding,                  //
    //               QSizePolicy::MinimumExpanding );                //
    //                                                               //
    //resize( 200, 200 );                                            //
    //                                                               //
    ///////////////////////////////////////////////////////////////////

    QList<QWidget *> focusChain;
    focusChain << this << d_data->titleLabel << getAxisWidget( xTop )
        << getAxisWidget( yLeft ) << getAxisWidget( yRight )
        << getAxisWidget( xBottom ) << d_data->footerLabel;

    for ( int i = 0; i < focusChain.size() - 1; i++ )
        qwtSetTabOrder( focusChain[i], focusChain[i+1], false );
}

AbstractAxis *SimplePlot::getAxisWidget(int axisId){
    return static_cast<AbstractAxis*>(d_axisData[axisId]->scaleWidget);
}

SimpleScaleWidget *SimplePlot::getSimpleScaleWidget(int axisId){
    return d_axisData[axisId]->scaleWidget;
}

const AbstractAxis *SimplePlot::getAxisWidget(int axisId) const {
    return static_cast<AbstractAxis*>(d_axisData[axisId]->scaleWidget);
}

const SimpleScaleWidget *SimplePlot::getSimpleScaleWidget(int axisId) const {
    return d_axisData[axisId]->scaleWidget;
}

AbstractPlotLayout *SimplePlot::getPlotLayout(){
    return static_cast<AbstractPlotLayout*>(d_data->layout);
}

const AbstractPlotLayout *SimplePlot::getPlotLayout() const {
    return static_cast<AbstractPlotLayout*>(d_data->layout);
}

void SimplePlot::setPlotLayout(AbstractPlotLayout *layout)
{
    delete d_data->layout;
    d_data->layout = layout;
}

/*!
  Change the plot's title
  \param title New title
*/
void SimplePlot::setTitle( const QString &title )
{
    if ( title != d_data->titleLabel->text().text() )
    {
        d_data->titleLabel->setText( title );
        updateLayout();
    }
}

/*!
  Change the plot's title
  \param title New title
*/
void SimplePlot::setTitle( const QwtText &title )
{
    if ( title != d_data->titleLabel->text() )
    {
        d_data->titleLabel->setText( title );
        updateLayout();
    }
}

//! \return Title of the plot
QwtText SimplePlot::title() const
{
    return d_data->titleLabel->text();
}

//! \return Title label widget.
QwtTextLabel *SimplePlot::titleLabel()
{
    return d_data->titleLabel;
}

//! \return Title label widget.
const QwtTextLabel *SimplePlot::titleLabel() const
{
    return d_data->titleLabel;
}

/*!
  Change the text the footer
  \param text New text of the footer
*/
void SimplePlot::setFooter( const QString &text )
{
    if ( text != d_data->footerLabel->text().text() )
    {
        d_data->footerLabel->setText( text );
        updateLayout();
    }
}

/*!
  Change the text the footer
  \param text New text of the footer
*/
void SimplePlot::setFooter( const QwtText &text )
{
    if ( text != d_data->footerLabel->text() )
    {
        d_data->footerLabel->setText( text );
        updateLayout();
    }
}

//! \return Text of the footer
QwtText SimplePlot::footer() const
{
    return d_data->footerLabel->text();
}

//! \return Footer label widget.
QwtTextLabel *SimplePlot::footerLabel()
{
    return d_data->footerLabel;
}

//! \return Footer label widget.
const QwtTextLabel *SimplePlot::footerLabel() const
{
    return d_data->footerLabel;
}

/*!
  \return Size hint for the plot widget
  \sa minimumSizeHint()
*/
QSize SimplePlot::sizeHint() const
{
    int dw = 0;
    int dh = 0;
    for ( int axisId = 0; axisId < axisCnt; axisId++ )
    {
        if ( isAxisEnabled( axisId ) )
        {
            const int niceDist = 40;
            const SimpleScaleWidget *scaleWidget = getSimpleScaleWidget( axisId );
            const QwtScaleDiv &scaleDiv = scaleWidget->scaleDraw()->scaleDiv();

            const int majCnt = scaleDiv.ticks( QwtScaleDiv::MajorTick ).count();

            if ( axisId == yLeft || axisId == yRight )
            {
                int hDiff = ( majCnt - 1 ) * niceDist
                    - scaleWidget->minimumSizeHint().height();
                if ( hDiff > dh )
                    dh = hDiff;
            }
            else
            {
                int wDiff = ( majCnt - 1 ) * niceDist
                    - scaleWidget->minimumSizeHint().width();
                if ( wDiff > dw )
                    dw = wDiff;
            }
        }
    }
    return minimumSizeHint() + QSize( dw, dh );
}

//////////////////////////////////////////////////////////////////
///*!                                                           //
//  \brief Return a minimum size hint                           //
//*                                                             //
//QSize SimplePlot::minimumSizeHint() const                     //
//{                                                             //
//    QSize hint = d_data->layout->minimumSizeHint( this );     //
//    hint += QSize( 2 * frameWidth(), 2 * frameWidth() );      //
//                                                              //
//    return hint;                                              //
//}                                                             //
//////////////////////////////////////////////////////////////////

/*!
  \brief Calculate the canvas margins

  \param maps AbstractPlot::axisCnt maps, mapping between plot and paint device coordinates
  \param canvasRect Bounding rectangle where to paint
  \param left Return parameter for the left margin
  \param top Return parameter for the top margin
  \param right Return parameter for the right margin
  \param bottom Return parameter for the bottom margin

  Plot items might indicate, that they need some extra space
  at the borders of the canvas by the QwtPlotItem::Margins flag.

  updateCanvasMargins(), QwtPlotItem::getCanvasMarginHint()
 */
void SimplePlot::getCanvasMarginsHint(
    const QwtScaleMap maps[], const QRectF &canvasRect,
    double &left, double &top, double &right, double &bottom) const
{

}

/*!
  \brief Update the canvas margins

  Plot items might indicate, that they need some extra space
  at the borders of the canvas by the QwtPlotItem::Margins flag.

  getCanvasMarginsHint(), QwtPlotItem::getCanvasMarginHint()
 */
void SimplePlot::updateCanvasMargins()
{
    QwtScaleMap maps[axisCnt];
    for ( int axisId = 0; axisId < axisCnt; axisId++ )
        maps[axisId] = canvasMap( axisId );

    double margins[axisCnt];
    getCanvasMarginsHint( maps, getCanvas()->contentsRect(),
        margins[yLeft], margins[xTop], margins[yRight], margins[xBottom] );

    bool doUpdate = false;
    for ( int axisId = 0; axisId < axisCnt; axisId++ )
    {
        if ( margins[axisId] >= 0.0 )
        {
            const int m = qCeil( margins[axisId] );
            getPlotLayout()->setCanvasMargin( m, axisId);
            doUpdate = true;
        }
    }

    if ( doUpdate )
        updateLayout();
}

/*!
  Redraw the canvas.
  \param painter Painter used for drawing

  \warning drawCanvas calls drawItems what is also used
           for printing. Applications that like to add individual
           plot items better overload drawItems()
  \sa drawItems()
*/
void SimplePlot::drawCanvas(QWidget *canvas, QPainter *painter )
{
    Q_UNUSED(canvas);
    QwtScaleMap maps[axisCnt];
    for ( int axisId = 0; axisId < axisCnt; axisId++ )
        maps[axisId] = canvasMap( axisId );

    drawItems( painter, getCanvas()->contentsRect(), maps );
}

/*!
  Redraw the canvas items.

  \param painter Painter used for drawing
  \param canvasRect Bounding rectangle where to paint
  \param maps AbstractPlot::axisCnt maps, mapping between plot and paint device coordinates

  \note Usually canvasRect is contentsRect() of the plot canvas.
        Due to a bug in Qt this rectangle might be wrong for certain
        frame styles ( f.e QFrame::Box ) and it might be necessary to
        fix the margins manually using QWidget::setContentsMargins()
*/

void SimplePlot::drawItems( QPainter *painter, const QRectF &canvasRect, const QwtScaleMap maps[axisCnt] ) const
{

}

/*!
 * \brief 计算用来映射canvas上某一方向（由坐标轴序号指定）的坐标和值的QwtScaleMap。
 * \param axisId 坐标轴序号。
 * \return canvas坐标和值的映射。使用该映射可以将真实数值转化为绘图坐标，反之亦然。
 * \sa QwtScaleMap, transform(), invTransform()
 * \todo 这里需要整理。
 */
QwtScaleMap SimplePlot::canvasMap( int axisId ) const
{
    return getSimpleScaleWidget(axisId)->caculateCanvasMap(getCanvas());
}

/*!
  \brief Change the background of the plotting area

  Sets brush to QPalette::Window of all color groups of
  the palette of the canvas. Using getCanvas()->setPalette()
  is a more powerful way to set these colors.

  \param brush New background brush
  \sa canvasBackground()
*/
void SimplePlot::setCanvasBackground( const QBrush &brush )
{
    QPalette pal = getCanvas()->palette();
    pal.setBrush( QPalette::Window, brush );

    getCanvas()->setPalette( pal );
}

/*!
  Nothing else than: getCanvas()->palette().brush(
        QPalette::Normal, QPalette::Window);

  \return Background brush of the plotting area.
  \sa setCanvasBackground()
*/
QBrush SimplePlot::canvasBackground() const
{
    return getCanvas()->palette().brush(
        QPalette::Normal, QPalette::Window );
}

//! Initialize axes
void SimplePlot::initAxesData()
{
    int axisId;

    for ( axisId = 0; axisId < axisCnt; axisId++ )
        d_axisData[axisId] = new AxisData;

    setAxisWidget(yLeft,new SimpleScaleWidget( AbstractAxis::LeftScale, this ));
    setAxisWidget(yRight,new SimpleScaleWidget( AbstractAxis::RightScale, this ));
    setAxisWidget(xTop,new SimpleScaleWidget( AbstractAxis::TopScale, this ));
    setAxisWidget(xBottom,new SimpleScaleWidget( AbstractAxis::BottomScale, this ));

    d_axisData[yLeft]->scaleWidget->setObjectName( "QwtPlotAxisYLeft" );
    d_axisData[yRight]->scaleWidget->setObjectName( "QwtPlotAxisYRight" );
    d_axisData[xTop]->scaleWidget->setObjectName( "QwtPlotAxisXTop" );
    d_axisData[xBottom]->scaleWidget->setObjectName( "QwtPlotAxisXBottom" );

#if 1
    // better find the font sizes from the application font
    QFont fscl( fontInfo().family(), 10 );
    QFont fttl( fontInfo().family(), 12, QFont::Bold );
#endif

    for ( axisId = 0; axisId < axisCnt; axisId++ )
    {
        AxisData &d = *d_axisData[axisId];

        getSimpleScaleWidget(axisId)->setScaleEngine(new QwtLinearScaleEngine());

        d.scaleWidget->setTransformation( getSimpleScaleWidget(axisId)->getScaleEngine()->transformation() );

        d.scaleWidget->setFont( fscl );
        d.scaleWidget->setMargin( 2 );

        d.doAutoScale = true;

        getSimpleScaleWidget(axisId)->setCanvasValueRange(0.0, 1000.0, 0.0);

        d.isValid = false;
    }

    d_axisData[yLeft]->scaleWidget->enableAxis(true);
    d_axisData[yRight]->scaleWidget->enableAxis(false);
    d_axisData[xBottom]->scaleWidget->enableAxis(true);
    d_axisData[xTop]->scaleWidget->enableAxis(false);
}

void SimplePlot::deleteAxesData()
{
    for ( int axisId = 0; axisId < axisCnt; axisId++ )
    {
        delete d_axisData[axisId];
        d_axisData[axisId] = NULL;
    }
}

/*!
  Change the scale engine for an axis

  \param axisId Axis index
  \param scaleEngine Scale engine

  \sa axisScaleEngine()
*/
void SimplePlot::setAxisScaleEngine( int axisId, QwtScaleEngine *scaleEngine )
{
    if ( axisValid( axisId ) && scaleEngine != NULL )
    {
        AxisData &d = *d_axisData[axisId];

        getSimpleScaleWidget(axisId)->setScaleEngine(scaleEngine);

        d_axisData[axisId]->scaleWidget->setTransformation(
            getSimpleScaleWidget(axisId)->getScaleEngine()->transformation() );

        d.isValid = false;

        autoRefresh();
    }
}

/*!
  \param axisId Axis index
  \return Scale engine for a specific axis
*/
QwtScaleEngine *SimplePlot::axisScaleEngine( int axisId )
{
    if ( axisValid( axisId ) )
        return getSimpleScaleWidget(axisId)->getScaleEngine();
    else
        return NULL;
}

/*!
  \param axisId Axis index
  \return Scale engine for a specific axis
*/
const QwtScaleEngine *SimplePlot::axisScaleEngine( int axisId ) const
{
    if ( axisValid( axisId ) )
        return getSimpleScaleWidget(axisId)->getScaleEngine();
    else
        return NULL;
}
/*!
  \return \c True, if autoscaling is enabled
  \param axisId Axis index
*/
bool SimplePlot::axisAutoScale( int axisId ) const
{
    if ( axisValid( axisId ) )
        return d_axisData[axisId]->doAutoScale;
    else
        return false;

}

/*!
  \return The font of the scale labels for a specified axis
  \param axisId Axis index
*/
QFont SimplePlot::axisFont( int axisId ) const
{
    if ( axisValid( axisId ) )
        return getSimpleScaleWidget( axisId )->font();
    else
        return QFont();

}

/*!
  \return The maximum number of major ticks for a specified axis
  \param axisId Axis index
  \sa setAxisMaxMajor(), QwtScaleEngine::divideScale()
*/
int SimplePlot::axisMaxMajor( int axisId ) const
{
    if ( axisValid( axisId ) )
        return getSimpleScaleWidget(axisId)->getMaxMajor();
    else
        return 0;
}

/*!
  \return the maximum number of minor ticks for a specified axis
  \param axisId Axis index
  \sa setAxisMaxMinor(), QwtScaleEngine::divideScale()
*/
int SimplePlot::axisMaxMinor( int axisId ) const
{
    if ( axisValid( axisId ) )
        return getSimpleScaleWidget(axisId)->getMaxMinor();
    else
        return 0;
}

/*!
  \brief Return the scale division of a specified axis

  axisScaleDiv(axisId).lowerBound(), axisScaleDiv(axisId).upperBound()
  are the current limits of the axis scale.

  \param axisId Axis index
  \return Scale division

  \sa QwtScaleDiv, setAxisScaleDiv(), QwtScaleEngine::divideScale()
*/
const QwtScaleDiv &SimplePlot::axisScaleDiv( int axisId ) const
{
    return getSimpleScaleWidget(axisId)->scaleDraw()->scaleDiv();
}

/*!
  \brief Return the scale draw of a specified axis

  \param axisId Axis index
  \return Specified scaleDraw for axis, or NULL if axis is invalid.
*/
const RectangularAxisDraw *SimplePlot::axisScaleDraw( int axisId ) const
{
    if ( !axisValid( axisId ) )
        return NULL;

    return getSimpleScaleWidget( axisId )->scaleDraw();
}

/*!
  \brief Return the scale draw of a specified axis

  \param axisId Axis index
  \return Specified scaleDraw for axis, or NULL if axis is invalid.
*/
RectangularAxisDraw *SimplePlot::axisScaleDraw( int axisId )
{
    if ( !axisValid( axisId ) )
        return NULL;

    return getSimpleScaleWidget( axisId )->scaleDraw();
}

/*!
  \brief Return the step size parameter that has been set in setAxisScale.

  This doesn't need to be the step size of the current scale.

  \param axisId Axis index
  \return step size parameter value

   \sa setAxisScale(), QwtScaleEngine::divideScale()
*/
double SimplePlot::axisStepSize( int axisId ) const
{
    if ( !axisValid( axisId ) )
        return 0;

    return getSimpleScaleWidget(axisId)->getStepSize();
}

/*!
  \brief Return the current interval of the specified axis

  This is only a convenience function for axisScaleDiv( axisId )->interval();

  \param axisId Axis index
  \return Scale interval

  \sa QwtScaleDiv, axisScaleDiv()
*/
QwtInterval SimplePlot::axisInterval( int axisId ) const
{
    if ( !axisValid( axisId ) )
        return QwtInterval();

    return getSimpleScaleWidget(axisId)->scaleDraw()->scaleDiv().interval();
}

/*!
  Transform the x or y coordinate of a position in the
  drawing region into a value.

  \param axisId Axis index
  \param pos position

  \return Position as axis coordinate

  \warning The position can be an x or a y coordinate,
           depending on the specified axis.
*/
double SimplePlot::invTransform( int axisId, int pos ) const
{
    if ( axisValid( axisId ) )
        return( canvasMap( axisId ).invTransform( pos ) );
    else
        return 0.0;
}

/*!
  \brief Transform a value into a coordinate in the plotting region

  \param axisId Axis index
  \param value value
  \return X or Y coordinate in the plotting region corresponding
          to the value.
*/
double SimplePlot::transform( int axisId, double value ) const
{
    if ( axisValid( axisId ) )
        return( canvasMap( axisId ).transform( value ) );
    else
        return 0.0;
}

void SimplePlot::createDefaultAxis(int axisId){
    switch(axisId){
    case xTop:
        setAxisWidget(axisId, new SimpleScaleWidget( AbstractAxis::TopScale, this ));
        break;
    case xBottom:
        setAxisWidget(axisId, new SimpleScaleWidget( AbstractAxis::BottomScale, this ));
        break;
    case yRight:
        setAxisWidget(axisId, new SimpleScaleWidget( AbstractAxis::RightScale, this ));
        break;
    case yLeft:
        setAxisWidget(axisId, new SimpleScaleWidget( AbstractAxis::LeftScale, this ));
        break;
    }
}

/*!
  \brief Change the font of an axis

  \param axisId Axis index
  \param font Font
  \warning This function changes the font of the tick labels,
           not of the axis title.
*/
void SimplePlot::setAxisFont( int axisId, const QFont &font )
{
    if ( axisValid( axisId ) )
        getSimpleScaleWidget( axisId )->setFont( font );
}

/*!
  \brief Enable autoscaling for a specified axis

  This member function is used to switch back to autoscaling mode
  after a fixed scale has been set. Autoscaling is enabled by default.

  \param axisId Axis index
  \param on On/Off
  \sa setAxisScale(), setAxisScaleDiv(), updateAxes()

  \note The autoscaling flag has no effect until updateAxes() is executed
        ( called by replot() ).
*/
void SimplePlot::setAxisAutoScale( int axisId, bool on )
{
    if ( axisValid( axisId ) && ( d_axisData[axisId]->doAutoScale != on ) )
    {
        d_axisData[axisId]->doAutoScale = on;
        autoRefresh();
    }
}

/*!
  \brief Disable autoscaling and specify a fixed scale for a selected axis.

  In updateAxes() the scale engine calculates a scale division from the
  specified parameters, that will be assigned to the scale widget. So
  updates of the scale widget usually happen delayed with the next replot.

  \param axisId Axis index
  \param min Minimum of the scale
  \param max Maximum of the scale
  \param stepSize Major step size. If <code>step == 0</code>, the step size is
                  calculated automatically using the maxMajor setting.

  \sa setAxisMaxMajor(), setAxisAutoScale(), axisStepSize(), QwtScaleEngine::divideScale()
*/
void SimplePlot::setAxisScale( int axisId, double min, double max, double stepSize )
{
    if ( axisValid( axisId ) )
    {
        AxisData &d = *d_axisData[axisId];

        d.doAutoScale = false;
        d.isValid = false;

        getSimpleScaleWidget(axisId)->setCanvasValueRange(min, max, stepSize);
        autoRefresh();
    }
}

/*!
  \brief Disable autoscaling and specify a fixed scale for a selected axis.

  The scale division will be stored locally only until the next call
  of updateAxes(). So updates of the scale widget usually happen delayed with
  the next replot.

  \param axisId Axis index
  \param scaleDiv Scale division

  \sa setAxisScale(), setAxisAutoScale()
*/
void SimplePlot::setAxisScaleDiv( int axisId, const QwtScaleDiv &scaleDiv )
{
    if ( axisValid( axisId ) )
    {
        AxisData &d = *d_axisData[axisId];

        d.doAutoScale = false;
        getSimpleScaleWidget(axisId)->setScaleDiv(scaleDiv);
        d.isValid = true;

        autoRefresh();
    }
}

/*!
  \brief Set a scale draw

  \param axisId Axis index
  \param scaleDraw Object responsible for drawing scales.

  By passing scaleDraw it is possible to extend RectangularAxisDraw
  functionality and let it take place in QwtPlot. Please note
  that scaleDraw has to be created with new and will be deleted
  by the corresponding QwtScale member ( like a child object ).

  \sa RectangularAxisDraw, AbstractAxis
  \warning The attributes of scaleDraw will be overwritten by those of the
           previous RectangularAxisDraw.
*/
void SimplePlot::setAxisScaleDraw( int axisId, RectangularAxisDraw *scaleDraw )
{
    if ( axisValid( axisId ) )
    {
        getSimpleScaleWidget( axisId )->setScaleDraw( scaleDraw );
        autoRefresh();
    }
}

void SimplePlot::setAxisWidget(int axisId, SimpleScaleWidget *scaleWidget){
    if ( axisValid( axisId ) ){
        scaleWidget->setParent(this);//Important!!!
        if(d_axisData[axisId]->scaleWidget!=NULL)
            delete d_axisData[axisId]->scaleWidget;
        d_axisData[axisId]->scaleWidget=scaleWidget;
        setAxisScaleDraw(axisId,scaleWidget->scaleDraw());

        switch(axisId){
        case yLeft:
            d_axisData[axisId]->scaleWidget->setObjectName( "QwtPlotAxisYLeft" );
            break;
        case yRight:
            d_axisData[axisId]->scaleWidget->setObjectName( "QwtPlotAxisYLeft" );
        case xTop:
            d_axisData[axisId]->scaleWidget->setObjectName( "QwtPlotAxisXTop" );
            break;
        case xBottom:
            d_axisData[axisId]->scaleWidget->setObjectName( "QwtPlotAxisXBottom" );
            break;
        }
        autoRefresh();
    }
}

/*!
  Change the alignment of the tick labels

  \param axisId Axis index
  \param alignment Or'd Qt::AlignmentFlags see <qnamespace.h>

  \sa RectangularAxisDraw::setLabelAlignment()
*/
void SimplePlot::setAxisLabelAlignment( int axisId, Qt::Alignment alignment )
{
    if ( axisValid( axisId ) )
        getSimpleScaleWidget( axisId )->setLabelAlignment( alignment );
}

/*!
  Rotate all tick labels

  \param axisId Axis index
  \param rotation Angle in degrees. When changing the label rotation,
                  the label alignment might be adjusted too.

  \sa RectangularAxisDraw::setLabelRotation(), setAxisLabelAlignment()
*/
void SimplePlot::setAxisLabelRotation( int axisId, double rotation )
{
    if ( axisValid( axisId ) )
        getSimpleScaleWidget( axisId )->setLabelRotation( rotation );
}

/*!
  Set the maximum number of minor scale intervals for a specified axis

  \param axisId Axis index
  \param maxMinor Maximum number of minor steps

  \sa axisMaxMinor()
*/
void SimplePlot::setAxisMaxMinor( int axisId, int maxMinor ){
    if ( axisValid( axisId ) ){
        getSimpleScaleWidget(axisId)->setMaxMajor(maxMinor);
    }
}

/*!
  Set the maximum number of major scale intervals for a specified axis

  \param axisId Axis index
  \param maxMajor Maximum number of major steps

  \sa axisMaxMajor()
*/
void SimplePlot::setAxisMaxMajor( int axisId, int maxMajor ){
    if ( axisValid( axisId ) ){
        getSimpleScaleWidget(axisId)->setMaxMajor(maxMajor);
    }
}

/*!
  \brief Rebuild the axes scales

  In case of autoscaling the boundaries of a scale are calculated
  from the bounding rectangles of all plot items, having the
  QwtPlotItem::AutoScale flag enabled ( QwtScaleEngine::autoScale() ).
  Then a scale division is calculated ( QwtScaleEngine::didvideScale() )
  and assigned to scale widget.

  When the scale boundaries have been assigned with setAxisScale() a
  scale division is calculated ( QwtScaleEngine::didvideScale() )
  for this interval and assigned to the scale widget.

  When the scale has been set explicitly by setAxisScaleDiv() the
  locally stored scale division gets assigned to the scale widget.

  The scale widget indicates modifications by emitting a
  AbstractAxis::scaleDivChanged() signal.

  updateAxes() is usually called by replot().

  \sa setAxisAutoScale(), setAxisScale(), setAxisScaleDiv(), replot()
      QwtPlotItem::boundingRect()
 */
void SimplePlot::updateAxes()
{
    /****************************************************************
    // Find bounding interval of the item data
    // for all axes, where autoscaling is enabled

    QwtInterval intv[axisCnt];

    const QwtPlotItemList& itmList = itemList();

    QwtPlotItemIterator it;
    for ( it = itmList.begin(); it != itmList.end(); ++it )
    {
        const QwtPlotItem *item = *it;

        if ( !item->testItemAttribute( QwtPlotItem::AutoScale ) )
            continue;

        if ( !item->isVisible() )
            continue;

        if ( axisAutoScale( item->xAxis() ) || axisAutoScale( item->yAxis() ) )
        {
            const QRectF rect = item->boundingRect();

            if ( rect.width() >= 0.0 )
                intv[item->xAxis()] |= QwtInterval( rect.left(), rect.right() );

            if ( rect.height() >= 0.0 )
                intv[item->yAxis()] |= QwtInterval( rect.top(), rect.bottom() );
        }
    }
    ********************************************************************/

    // Adjust scales
    for ( int axisId = 0; axisId < axisCnt; axisId++ ){
        SimpleScaleWidget *scaleWidget = getSimpleScaleWidget( axisId );
        if(scaleWidget)
            scaleWidget->updateScaleDiv();
    }
    /********************************************************************
    for ( it = itmList.begin(); it != itmList.end(); ++it )
    {
        QwtPlotItem *item = *it;
        if ( item->testItemInterest( QwtPlotItem::ScaleInterest ) )
        {
            item->updateScaleDiv( axisScaleDiv( item->xAxis() ),
                axisScaleDiv( item->yAxis() ) );
        }
    }
    ********************************************************************/
}


