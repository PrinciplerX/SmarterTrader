#include "simpleplotlayout.h"
#include "qwt_text.h"
#include "qwt_text_label.h"
#include "simplescalewidget.h"
#include "qwt_abstract_legend.h"
#include <qscrollbar.h>
#include <qmath.h>

class SimplePlotLayout::LayoutData
{
public:
    void init( const SimplePlot *, const QRectF &rect );

    struct t_legendData{
        int frameWidth;                         //space needed to draw window frame
        int hScrollExtent;                      //space needed to show vertical scroll bar
        int vScrollExtent;                      //space needed to show horizontal scroll bar
        QSize hint;                             //recommended size for the the legend widget
    } legend;

    struct t_titleData{
        QwtText text;                           //title text
        int frameWidth;                         //space needed to draw window frame
    } title;

    struct t_footerData{
        QwtText text;                           //footer text
        int frameWidth;                         //space needed to draw window frame
    } footer;

    struct t_scaleData{
        bool isEnabled;                         //true if scale widget show in the plot
        const SimpleScaleWidget *scaleWidget;   //scale widget
        QFont scaleFont;                        //scale widget label font
        int baseLineOffset;                     //
        double tickOffset;                      //
        int dimWithoutTitle;                    //
    } scale[AbstractPlot::axisCnt];

    struct t_canvasData{
        int contentsMargins[ AbstractPlot::axisCnt ];
    } canvas;
};

/*!
 * \brief 从SimplePlot对象和该对象的目标显示区域初始化关于布局的所有相关信息。
 * \param plot 要布局的SimplePlot对象。
 * \param rect SimplePlot的目标显示区域。
 */
void SimplePlotLayout::LayoutData::init( const SimplePlot *plot, const QRectF &rect ){
    // legend
    if (plot->legend() != nullptr){
        legend.frameWidth = plot->legend()->frameWidth();

        legend.hScrollExtent = plot->legend()->scrollExtent( Qt::Horizontal );
        legend.vScrollExtent = plot->legend()->scrollExtent( Qt::Vertical );

        const QSize hint = plot->legend()->sizeHint();

        //legend大小不能超过SimplePlot
        int w = qMin( hint.width(), qFloor( rect.width() ) );
        int h = plot->legend()->heightForWidth( w );
        if ( h <= 0 )
            h = hint.height();

        //如果竖直方向上需要显示的内容超过了显示区域大小，则需要显示竖直滚动条
        if ( h > rect.height() )
            w += legend.hScrollExtent;

        legend.hint = QSize( w, h );
    }

    // title
    title.frameWidth = 0;
    title.text = QwtText();

    if (plot->titleLabel() != nullptr){
        const QwtTextLabel *label = plot->titleLabel();
        title.text = label->text();
        if ( !( title.text.testPaintAttribute( QwtText::PaintUsingTextFont ) ) )
            title.text.setFont( label->font() );

        title.frameWidth = plot->titleLabel()->frameWidth();
    }

    // footer
    footer.frameWidth = 0;
    footer.text = QwtText();

    if (plot->footerLabel() != nullptr){
        const QwtTextLabel *label = plot->footerLabel();
        footer.text = label->text();
        if ( !( footer.text.testPaintAttribute( QwtText::PaintUsingTextFont ) ) )
            footer.text.setFont( label->font() );

        footer.frameWidth = plot->footerLabel()->frameWidth();
    }

    // scales
    for ( int axis = 0; axis < AbstractPlot::axisCnt; axis++ ){
        if (plot->isAxisEnabled(axis)){
            const SimpleScaleWidget *scaleWidget = plot->getSimpleScaleWidget( axis );

            scale[axis].isEnabled = true;
            scale[axis].scaleWidget = scaleWidget;
            scale[axis].scaleFont = scaleWidget->font();
            scale[axis].baseLineOffset = scaleWidget->getMargin();
            scale[axis].tickOffset = scaleWidget->getMargin();

            if(scaleWidget->scaleDraw()->hasComponent(AbstractScaleDraw::Ticks)){
                scale[axis].tickOffset += scaleWidget->scaleDraw()->maxTickLength();
            }

            scale[axis].dimWithoutTitle = scaleWidget->dimForLength(
                QWIDGETSIZE_MAX, scale[axis].scaleFont );

            if (!scaleWidget->title().isEmpty()){
                scale[axis].dimWithoutTitle -=
                    scaleWidget->titleHeightForWidth( QWIDGETSIZE_MAX );
            }
        }else{
            scale[axis].isEnabled = false;
            scale[axis].baseLineOffset = 0;
            scale[axis].tickOffset = 0.0;
            scale[axis].dimWithoutTitle = 0;
        }
    }

    // canvas
    plot->getCanvas()->getContentsMargins(
        &canvas.contentsMargins[ AbstractPlot::yLeft ],
        &canvas.contentsMargins[ AbstractPlot::xTop ],
        &canvas.contentsMargins[ AbstractPlot::yRight ],
        &canvas.contentsMargins[ AbstractPlot::xBottom ] );
}

class SimplePlotLayout::PrivateData
{
public:
    PrivateData():
        spacing( 5 )
    {
    }

    QRectF titleRect;
    QRectF footerRect;
    QRectF legendRect;

    SimplePlotLayout::LayoutData layoutData;

    SimplePlot::LegendPosition legendPos;
    double legendRatio;
    unsigned int spacing;
};

/*!
  \brief Constructor
 */

SimplePlotLayout::SimplePlotLayout()
{
    d_data = new PrivateData;

    setLegendPosition( SimplePlot::BottomLegend );
    setCanvasMargin( 4 );
    setAlignCanvasToScales( false );

    invalidate();
}

//! Destructor
SimplePlotLayout::~SimplePlotLayout()
{
    delete d_data;
}

/*!
  Change the spacing of the plot. The spacing is the distance
  between the plot components.

  \param spacing New spacing
  \sa setCanvasMargin(), spacing()
*/
void SimplePlotLayout::setSpacing( int spacing )
{
    d_data->spacing = qMax( 0, spacing );
}

/*!
  \return Spacing
  \sa margin(), setSpacing()
*/
int SimplePlotLayout::spacing() const
{
    return d_data->spacing;
}

/*!
  \brief Specify the position of the legend
  \param pos The legend's position.
  \param ratio Ratio between legend and the bounding rectangle
               of title, footer, canvas and axes. The legend will be shrunk
               if it would need more space than the given ratio.
               The ratio is limited to ]0.0 .. 1.0]. In case of <= 0.0
               it will be reset to the default ratio.
               The default vertical/horizontal ratio is 0.33/0.5.

  \sa SimplePlot::setLegendPosition()
*/

void SimplePlotLayout::setLegendPosition( SimplePlot::LegendPosition pos, double ratio )
{
    if ( ratio > 1.0 )
        ratio = 1.0;

    switch ( pos )
    {
        case SimplePlot::TopLegend:
        case SimplePlot::BottomLegend:
            if ( ratio <= 0.0 )
                ratio = 0.33;
            d_data->legendRatio = ratio;
            d_data->legendPos = pos;
            break;
        case SimplePlot::LeftLegend:
        case SimplePlot::RightLegend:
            if ( ratio <= 0.0 )
                ratio = 0.5;
            d_data->legendRatio = ratio;
            d_data->legendPos = pos;
            break;
        default:
            break;
    }
}

/*!
  \brief Specify the position of the legend
  \param pos The legend's position. Valid values are
      \c SimplePlot::LeftLegend, \c SimplePlot::RightLegend,
      \c SimplePlot::TopLegend, \c SimplePlot::BottomLegend.

  \sa SimplePlot::setLegendPosition()
*/
void SimplePlotLayout::setLegendPosition( SimplePlot::LegendPosition pos )
{
    setLegendPosition( pos, 0.0 );
}

/*!
  \return Position of the legend
  \sa setLegendPosition(), SimplePlot::setLegendPosition(),
      SimplePlot::legendPosition()
*/
SimplePlot::LegendPosition SimplePlotLayout::legendPosition() const
{
    return d_data->legendPos;
}

/*!
  Specify the relative size of the legend in the plot
  \param ratio Ratio between legend and the bounding rectangle
               of title, footer, canvas and axes. The legend will be shrunk
               if it would need more space than the given ratio.
               The ratio is limited to ]0.0 .. 1.0]. In case of <= 0.0
               it will be reset to the default ratio.
               The default vertical/horizontal ratio is 0.33/0.5.
*/
void SimplePlotLayout::setLegendRatio( double ratio )
{
    setLegendPosition( legendPosition(), ratio );
}

/*!
  \return The relative size of the legend in the plot.
  \sa setLegendPosition()
*/
double SimplePlotLayout::legendRatio() const
{
    return d_data->legendRatio;
}

/*!
  \brief Set the geometry for the title

  This method is intended to be used from derived layouts
  overloading activate()

  \sa titleRect(), activate()
 */
void SimplePlotLayout::setTitleRect( const QRectF &rect )
{
    d_data->titleRect = rect;
}

/*!
  \return Geometry for the title
  \sa activate(), invalidate()
*/
QRectF SimplePlotLayout::titleRect() const
{
    return d_data->titleRect;
}

/*!
  \brief Set the geometry for the footer

  This method is intended to be used from derived layouts
  overloading activate()

  \sa footerRect(), activate()
 */
void SimplePlotLayout::setFooterRect( const QRectF &rect )
{
    d_data->footerRect = rect;
}

/*!
  \return Geometry for the footer
  \sa activate(), invalidate()
*/
QRectF SimplePlotLayout::footerRect() const
{
    return d_data->footerRect;
}

/*!
  \brief Set the geometry for the legend

  This method is intended to be used from derived layouts
  overloading activate()

  \param rect Rectangle for the legend

  \sa legendRect(), activate()
 */
void SimplePlotLayout::setLegendRect( const QRectF &rect )
{
    d_data->legendRect = rect;
}

/*!
  \return Geometry for the legend
  \sa activate(), invalidate()
*/
QRectF SimplePlotLayout::legendRect() const
{
    return d_data->legendRect;
}

/*!
  Invalidate the geometry of all components.
  \sa activate()
*/
void SimplePlotLayout::invalidate()
{
    d_data->titleRect = d_data->footerRect = d_data->legendRect = QRect();
}

/*!
  \return Minimum size hint
  \param plot Plot widget

  \sa SimplePlot::minimumSizeHint()
*/

QSize SimplePlotLayout::minimumSizeHint( const SimplePlot *plot ) const
{
    class ScaleData
    {
    public:
        ScaleData()
        {
            w = h = minLeft = minRight = tickOffset = 0;
        }

        int w;
        int h;
        int minLeft;
        int minRight;
        int tickOffset;
    } scaleData[AbstractPlot::axisCnt];

    int canvasBorder[AbstractPlot::axisCnt];

    int fw;
    plot->getCanvas()->getContentsMargins( &fw, NULL, NULL, NULL );

    int axis;
    for ( axis = 0; axis < AbstractPlot::axisCnt; axis++ )
    {
        if ( plot->isAxisEnabled( axis ) )
        {
            const SimpleScaleWidget *scl = plot->getSimpleScaleWidget( axis );
            ScaleData &sd = scaleData[axis];

            const QSize hint = scl->minimumSizeHint();
            sd.w = hint.width();
            sd.h = hint.height();
            scl->getBorderDistHint( sd.minLeft, sd.minRight );
            sd.tickOffset = scl->getMargin();
            if ( scl->scaleDraw()->hasComponent( AbstractScaleDraw::Ticks ) )
                sd.tickOffset += qCeil( scl->scaleDraw()->maxTickLength() );
        }

        canvasBorder[axis] = fw + getCanvasMargin(axis) + 1;
    }


    for ( axis = 0; axis < AbstractPlot::axisCnt; axis++ )
    {
        ScaleData &sd = scaleData[axis];
        if ( sd.w && ( axis == AbstractPlot::xBottom || axis == AbstractPlot::xTop ) )
        {
            if ( ( sd.minLeft > canvasBorder[AbstractPlot::yLeft] )
                && scaleData[AbstractPlot::yLeft].w )
            {
                int shiftLeft = sd.minLeft - canvasBorder[AbstractPlot::yLeft];
                if ( shiftLeft > scaleData[AbstractPlot::yLeft].w )
                    shiftLeft = scaleData[AbstractPlot::yLeft].w;

                sd.w -= shiftLeft;
            }
            if ( ( sd.minRight > canvasBorder[AbstractPlot::yRight] )
                && scaleData[AbstractPlot::yRight].w )
            {
                int shiftRight = sd.minRight - canvasBorder[AbstractPlot::yRight];
                if ( shiftRight > scaleData[AbstractPlot::yRight].w )
                    shiftRight = scaleData[AbstractPlot::yRight].w;

                sd.w -= shiftRight;
            }
        }

        if ( sd.h && ( axis == AbstractPlot::yLeft || axis == AbstractPlot::yRight ) )
        {
            if ( ( sd.minLeft > canvasBorder[AbstractPlot::xBottom] ) &&
                scaleData[AbstractPlot::xBottom].h )
            {
                int shiftBottom = sd.minLeft - canvasBorder[AbstractPlot::xBottom];
                if ( shiftBottom > scaleData[AbstractPlot::xBottom].tickOffset )
                    shiftBottom = scaleData[AbstractPlot::xBottom].tickOffset;

                sd.h -= shiftBottom;
            }
            if ( ( sd.minLeft > canvasBorder[AbstractPlot::xTop] ) &&
                scaleData[AbstractPlot::xTop].h )
            {
                int shiftTop = sd.minRight - canvasBorder[AbstractPlot::xTop];
                if ( shiftTop > scaleData[AbstractPlot::xTop].tickOffset )
                    shiftTop = scaleData[AbstractPlot::xTop].tickOffset;

                sd.h -= shiftTop;
            }
        }
    }

    const QWidget *canvas = plot->getCanvas();

    int left, top, right, bottom;
    canvas->getContentsMargins( &left, &top, &right, &bottom );

    const QSize minCanvasSize = canvas->minimumSize();

    int w = scaleData[AbstractPlot::yLeft].w + scaleData[AbstractPlot::yRight].w;
    int cw = qMax( scaleData[AbstractPlot::xBottom].w, scaleData[AbstractPlot::xTop].w )
        + left + 1 + right + 1;
    w += qMax( cw, minCanvasSize.width() );

    int h = scaleData[AbstractPlot::xBottom].h + scaleData[AbstractPlot::xTop].h;
    int ch = qMax( scaleData[AbstractPlot::yLeft].h, scaleData[AbstractPlot::yRight].h )
        + top + 1 + bottom + 1;
    h += qMax( ch, minCanvasSize.height() );

    const QwtTextLabel *labels[2];
    labels[0] = plot->titleLabel();
    labels[1] = plot->footerLabel();

    for ( int i = 0; i < 2; i++ )
    {
        const QwtTextLabel *label   = labels[i];
        if ( label && !label->text().isEmpty() )
        {
            // If only AbstractPlot::yLeft or AbstractPlot::yRight is showing,
            // we center on the plot canvas.
            const bool centerOnCanvas = !( plot->isAxisEnabled( AbstractPlot::yLeft )
                && plot->isAxisEnabled( AbstractPlot::yRight ) );

            int labelW = w;
            if ( centerOnCanvas )
            {
                labelW -= scaleData[AbstractPlot::yLeft].w
                    + scaleData[AbstractPlot::yRight].w;
            }

            int labelH = label->heightForWidth( labelW );
            if ( labelH > labelW ) // Compensate for a long title
            {
                w = labelW = labelH;
                if ( centerOnCanvas )
                {
                    w += scaleData[AbstractPlot::yLeft].w
                        + scaleData[AbstractPlot::yRight].w;
                }

                labelH = label->heightForWidth( labelW );
            }
            h += labelH + d_data->spacing;
        }
    }

    // Compute the legend contribution

    const QwtAbstractLegend *legend = plot->legend();
    if ( legend && !legend->isEmpty() )
    {
        if ( d_data->legendPos == SimplePlot::LeftLegend
            || d_data->legendPos == SimplePlot::RightLegend )
        {
            int legendW = legend->sizeHint().width();
            int legendH = legend->heightForWidth( legendW );

            if ( legend->frameWidth() > 0 )
                w += d_data->spacing;

            if ( legendH > h )
                legendW += legend->scrollExtent( Qt::Horizontal );

            if ( d_data->legendRatio < 1.0 )
                legendW = qMin( legendW, int( w / ( 1.0 - d_data->legendRatio ) ) );

            w += legendW + d_data->spacing;
        }
        else // SimplePlot::Top, SimplePlot::Bottom
        {
            int legendW = qMin( legend->sizeHint().width(), w );
            int legendH = legend->heightForWidth( legendW );

            if ( legend->frameWidth() > 0 )
                h += d_data->spacing;

            if ( d_data->legendRatio < 1.0 )
                legendH = qMin( legendH, int( h / ( 1.0 - d_data->legendRatio ) ) );

            h += legendH + d_data->spacing;
        }
    }

    return QSize( w, h );
}

/*!
  Find the geometry for the legend

  \param options Options how to layout the legend
  \param rect Rectangle where to place the legend

  \return Geometry for the legend
  \sa Options
*/

QRectF SimplePlotLayout::layoutLegend( Options options,
    const QRectF &rect ) const
{
    const QSize hint( d_data->layoutData.legend.hint );

    int dim;
    if ( d_data->legendPos == SimplePlot::LeftLegend
        || d_data->legendPos == SimplePlot::RightLegend )
    {
        // We don't allow vertical legends to take more than
        // half of the available space.

        dim = qMin( hint.width(), int( rect.width() * d_data->legendRatio ) );

        if ( !( options & IgnoreScrollbars ) )
        {
            if ( hint.height() > rect.height() )
            {
                // The legend will need additional
                // space for the vertical scrollbar.

                dim += d_data->layoutData.legend.hScrollExtent;
            }
        }
    }
    else
    {
        dim = qMin( hint.height(), int( rect.height() * d_data->legendRatio ) );
        dim = qMax( dim, d_data->layoutData.legend.vScrollExtent );
    }

    QRectF legendRect = rect;
    switch ( d_data->legendPos )
    {
        case SimplePlot::LeftLegend:
            legendRect.setWidth( dim );
            break;
        case SimplePlot::RightLegend:
            legendRect.setX( rect.right() - dim );
            legendRect.setWidth( dim );
            break;
        case SimplePlot::TopLegend:
            legendRect.setHeight( dim );
            break;
        case SimplePlot::BottomLegend:
            legendRect.setY( rect.bottom() - dim );
            legendRect.setHeight( dim );
            break;
    }

    return legendRect;
}

/*!
  Align the legend to the canvas

  \param canvasRect Geometry of the canvas
  \param legendRect Maximum geometry for the legend

  \return Geometry for the aligned legend
*/
QRectF SimplePlotLayout::alignLegend( const QRectF &canvasRect,
    const QRectF &legendRect ) const
{
    QRectF alignedRect = legendRect;

    if ( d_data->legendPos == SimplePlot::BottomLegend
        || d_data->legendPos == SimplePlot::TopLegend )
    {
        if ( d_data->layoutData.legend.hint.width() < canvasRect.width() )
        {
            alignedRect.setX( canvasRect.x() );
            alignedRect.setWidth( canvasRect.width() );
        }
    }
    else
    {
        if ( d_data->layoutData.legend.hint.height() < canvasRect.height() )
        {
            alignedRect.setY( canvasRect.y() );
            alignedRect.setHeight( canvasRect.height() );
        }
    }

    return alignedRect;
}

/*!
  Expand all line breaks in text labels, and calculate the height
  of their widgets in orientation of the text.

  \param options Options how to layout the legend
  \param rect Bounding rectangle for title, footer, axes and canvas.
  \param dimTitle Expanded height of the title widget
  \param dimFooter Expanded height of the footer widget
  \param dimAxis Expanded heights of the axis in axis orientation.

  \sa Options
*/
void SimplePlotLayout::expandLineBreaks( Options options, const QRectF &rect,
    int &dimTitle, int &dimFooter, int dimAxis[AbstractPlot::axisCnt] ) const
{
    dimTitle = dimFooter = 0;

    int backboneOffset[AbstractPlot::axisCnt];
    for ( int axis = 0; axis < AbstractPlot::axisCnt; axis++ )
    {
        backboneOffset[axis] = 0;
        if ( !( options & IgnoreFrames ) )
            backboneOffset[axis] += d_data->layoutData.canvas.contentsMargins[ axis ];

        if ( !isCanvasAlignedToScale(axis) )
            backboneOffset[axis] += getCanvasMargin(axis);
    }

    bool done = false;
    while ( !done )
    {
        done = true;

        // the size for the 4 axis depend on each other. Expanding
        // the height of a horizontal axis will shrink the height
        // for the vertical axis, shrinking the height of a vertical
        // axis will result in a line break what will expand the
        // width and results in shrinking the width of a horizontal
        // axis what might result in a line break of a horizontal
        // axis ... . So we loop as long until no size changes.

        if ( !( ( options & IgnoreTitle ) ||
            d_data->layoutData.title.text.isEmpty() ) )
        {
            double w = rect.width();

            if ( d_data->layoutData.scale[AbstractPlot::yLeft].isEnabled
                != d_data->layoutData.scale[AbstractPlot::yRight].isEnabled )
            {
                // center to the canvas
                w -= (dimAxis[AbstractPlot::yLeft] + dimAxis[AbstractPlot::yRight]);
            }

            int d = qCeil( d_data->layoutData.title.text.heightForWidth( w ) );
            if ( !( options & IgnoreFrames ) )
                d += 2 * d_data->layoutData.title.frameWidth;

            if ( d > dimTitle ){
                dimTitle = d;
                done = false;
            }
        }

        if ( !( ( options & IgnoreFooter ) ||
            d_data->layoutData.footer.text.isEmpty() ) ){
            double w = rect.width();

            if ( d_data->layoutData.scale[AbstractPlot::yLeft].isEnabled
                != d_data->layoutData.scale[AbstractPlot::yRight].isEnabled )
            {
                // center to the canvas
                w -= (dimAxis[AbstractPlot::yLeft] + dimAxis[AbstractPlot::yRight]);
            }

            int d = qCeil( d_data->layoutData.footer.text.heightForWidth( w ) );
            if ( !( options & IgnoreFrames ) )
                d += 2 * d_data->layoutData.footer.frameWidth;

            if ( d > dimFooter )
            {
                dimFooter = d;
                done = false;
            }
        }

        //删除了部分不需要的内容
    }
}

/*!
  \brief Recalculate the geometry of all components.

  \param plot Plot to be layout
  \param plotRect Rectangle where to place the components
  \param options Layout options

  \sa invalidate(), titleRect(), footerRect()
      legendRect(), scaleRect(), canvasRect()
*/
void SimplePlotLayout::activate(const SimplePlot *plot, const QRectF &plotRect)
{
    invalidate();

    QRectF rect( plotRect );  // undistributed rest of the plot rect

    // We extract all layout relevant parameters from the widgets,
    // and save them to d_data->layoutData.

    d_data->layoutData.init( plot, rect );

    //legend
    if (!(layoutOptions&IgnoreLegend) && plot->legend() && !plot->legend()->isEmpty()){
        d_data->legendRect = layoutLegend( layoutOptions, rect );

        // subtract d_data->legendRect from rect

        const QRegion region( rect.toRect() );
        rect = region.subtracted( d_data->legendRect.toRect() ).boundingRect();

        switch ( d_data->legendPos ){
            case SimplePlot::LeftLegend:
                rect.setLeft( rect.left() + d_data->spacing );
                break;
            case SimplePlot::RightLegend:
                rect.setRight( rect.right() - d_data->spacing );
                break;
            case SimplePlot::TopLegend:
                rect.setTop( rect.top() + d_data->spacing );
                break;
            case SimplePlot::BottomLegend:
                rect.setBottom( rect.bottom() - d_data->spacing );
                break;
        }
    }

    /*
     +---+-----------+---+
     |       Title       |
     +---+-----------+---+
     |   |   Axis    |   |
     +---+-----------+---+
     | A |           | A |
     | x |  Canvas   | x |
     | i |           | i |
     | s |           | s |
     +---+-----------+---+
     |   |   Axis    |   |
     +---+-----------+---+
     |      Footer       |
     +---+-----------+---+
    */

    // title, footer and axes include text labels. The height of each
    // label depends on its line breaks, that depend on the width
    // for the label. A line break in a horizontal text will reduce
    // the available width for vertical texts and vice versa.
    // expandLineBreaks finds the height/width for title, footer and axes
    // including all line breaks.

    int dimTitle;
    int dimFooter;

    int dimAxes[AbstractPlot::axisCnt];//各个Axis的宽度（tick方向上）
    for(int i=AbstractPlot::yLeft; i<AbstractPlot::axisCnt; i++){
        if(plot->isAxisEnabled(i))
            dimAxes[i] = plot->getAxisWidget(i)->dimHint();
        else
            dimAxes[i] = 0;
    }

    expandLineBreaks( layoutOptions, rect, dimTitle, dimFooter, dimAxes );

    if ( dimTitle > 0 ){
        d_data->titleRect.setRect(
            rect.left(), rect.top(), rect.width(), dimTitle );

        rect.setTop( d_data->titleRect.bottom() + d_data->spacing );

        if ( d_data->layoutData.scale[AbstractPlot::yLeft].isEnabled !=
            d_data->layoutData.scale[AbstractPlot::yRight].isEnabled )
        {
            // if only one of the y axes is missing we align
            // the title centered to the canvas

            d_data->titleRect.setX( rect.left() + dimAxes[AbstractPlot::yLeft] );
            d_data->titleRect.setWidth( rect.width()
                - dimAxes[AbstractPlot::yLeft] - dimAxes[AbstractPlot::yRight] );
        }
    }

    if ( dimFooter > 0 ){
        d_data->footerRect.setRect(
            rect.left(), rect.bottom() - dimFooter, rect.width(), dimFooter );

        rect.setBottom( d_data->footerRect.top() - d_data->spacing );

        if ( d_data->layoutData.scale[AbstractPlot::yLeft].isEnabled !=
            d_data->layoutData.scale[AbstractPlot::yRight].isEnabled )
        {
            // if only one of the y axes is missing we align
            // the footer centered to the canvas

            d_data->footerRect.setX( rect.left() + dimAxes[AbstractPlot::yLeft] );
            d_data->footerRect.setWidth( rect.width()
                - dimAxes[AbstractPlot::yLeft] - dimAxes[AbstractPlot::yRight] );
        }
    }

    AbstractPlotLayout::activate(plot, rect);

    if ( !d_data->legendRect.isEmpty() ){
        // We prefer to align the legend to the canvas - not to
        // the complete plot - if possible.

        d_data->legendRect = alignLegend( getCanvasRect(), d_data->legendRect );
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*==========================================================================================================*/
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
