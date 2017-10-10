#include "abstractplotlayout.h"
#include "qwt_text.h"
#include "qwt_text_label.h"
#include "simplescalewidget.h"
#include <qscrollbar.h>
#include <qmath.h>

class AbstractPlotLayout::LayoutData
{
public:
    void init(const AbstractPlot *);

    struct t_scaleData{
        bool isEnabled;                         //true if scale widget show in the plot
        int start;                              //start border dist
        int end;                                //end border dist
        double tickOffset;                      //
    } scale[AbstractPlot::axisCnt];

    struct t_canvasData{
        // 画布可能是有边框的，画布的内容和画布控件边界的之间的部分就是画布边框的区域。
        // 布局使要考虑到这一点，否则会导致坐标轴和画布没有对齐。
        // 注意，canvas contents margin和canvas margin是不同的。
        int contentsMargins[ AbstractPlot::axisCnt ];
    } canvas;
};

/*!
 * \brief 从SimplePlot对象和该对象的目标显示区域初始化关于布局的所有相关信息。
 * \param plot 要布局的SimplePlot对象。
 * \param rect SimplePlot的目标显示区域。
 */
void AbstractPlotLayout::LayoutData::init(const AbstractPlot *plot){
    // scales
    for ( int axis = 0; axis < AbstractPlot::axisCnt; axis++ ){
        if (plot->isAxisEnabled(axis)){
            const AbstractAxis *scaleWidget = plot->getAxisWidget( axis );

            scale[axis].isEnabled = true;
            scale[axis].start = scaleWidget->getStartBorderDist();
            scale[axis].end = scaleWidget->getEndBorderDist();

        }else{
            scale[axis].isEnabled = false;
            scale[axis].start = 0;
            scale[axis].end = 0;
        }
    }

    // canvas
    plot->getCanvas()->getContentsMargins(
        &canvas.contentsMargins[ AbstractPlot::yLeft ],
        &canvas.contentsMargins[ AbstractPlot::xTop ],
        &canvas.contentsMargins[ AbstractPlot::yRight ],
        &canvas.contentsMargins[ AbstractPlot::xBottom ] );
}

class AbstractPlotLayout::PrivateData
{
public:
    AbstractPlotLayout::LayoutData layoutData;
};

AbstractPlotLayout::AbstractPlotLayout()
{
    d_data = new PrivateData;

    setCanvasMargin( 4 );
    setAlignCanvasToScales( false );

    invalidate();
}

AbstractPlotLayout::~AbstractPlotLayout()
{
    delete d_data;
}

/*!
 * Change a margin of the canvas. The margin is the space
  above/below the scale ticks. A negative margin will
  be set to -1, excluding the borders of the scales.

  \param margin New margin
  \param axis One of SimplePlot::Axis. Specifies where the position of the margin.
              -1 means margin at all borders.
  \sa canvasMargin()

  \warning The margin will have no effect when alignCanvasToScale() is true
*/
/*!
 * \brief 更改画布的margin。
 * \param margin
 * \param axis
 *
 * \sa getCanvasMargin()
 * \warning 如果isCanvasAlignToScale()为true时，margin无效。
 */
void AbstractPlotLayout::setCanvasMargin( int margin, int axis )
{
    if ( margin < -1 )
        margin = -1;

    if ( axis == -1 )
    {
        for ( axis = 0; axis < AbstractPlot::axisCnt; axis++ )
            canvasMargin[axis] = margin;
    }
    else if ( axis >= 0 && axis < AbstractPlot::axisCnt )
        canvasMargin[axis] = margin;
}

/*!
    \param axisId Axis index
    \return Margin around the scale tick borders
    \sa setCanvasMargin()
*/
int AbstractPlotLayout::getCanvasMargin( int axisId ) const
{
    if ( axisId < 0 || axisId >= AbstractPlot::axisCnt )
        return 0;

    return canvasMargin[axisId];
}

/*!
  \brief Set the align-canvas-to-axis-scales flag for all axes

  \param on True/False
  \sa setAlignCanvasToScale(), alignCanvasToScale()
*/
void AbstractPlotLayout::setAlignCanvasToScales( bool on )
{
    for ( int axis = 0; axis < AbstractPlot::axisCnt; axis++ )
        alignCanvasToScales[axis] = on;
}

/*!
  Change the align-canvas-to-axis-scales setting. The canvas may:

  - extend beyond the axis scale ends to maximize its size,
  - align with the axis scale ends to control its size.

  The axisId parameter is somehow confusing as it identifies a border
  of the plot and not the axes, that are aligned. F.e when AbstractPlot::yLeft
  is set, the left end of the the x-axes ( AbstractPlot::xTop, AbstractPlot::xBottom )
  is aligned.

  \param axisId Axis index
  \param on New align-canvas-to-axis-scales setting

  \sa setCanvasMargin(), alignCanvasToScale(), setAlignCanvasToScales()
  \warning In case of on == true canvasMargin() will have no effect
*/
void AbstractPlotLayout::setAlignCanvasToScale( int axisId, bool on )
{
    if ( axisId >= 0 && axisId < AbstractPlot::axisCnt )
        alignCanvasToScales[axisId] = on;
}

/*!
  Return the align-canvas-to-axis-scales setting. The canvas may:
  - extend beyond the axis scale ends to maximize its size
  - align with the axis scale ends to control its size.

  \param axisId Axis index
  \return align-canvas-to-axis-scales setting
  \sa setAlignCanvasToScale(), setAlignCanvasToScale(), setCanvasMargin()
*/
bool AbstractPlotLayout::isCanvasAlignedToScale( int axisId ) const
{
    if ( axisId < 0 || axisId >= AbstractPlot::axisCnt )
        return false;

    return alignCanvasToScales[ axisId ];
}

void AbstractPlotLayout::setLayoutOption(AbstractPlotLayout::LayoutOption option)
{
    layoutOption = option;
}

void AbstractPlotLayout::setSpacing( int spacing ){
    spacing = qMax( 0, spacing );
}

int AbstractPlotLayout::getSpacing() const {
    return spacing;
}

/*!
  \brief Set the geometry for an axis

  This method is intended to be used from derived layouts
  overloading activate()

  \param axis Axis index
  \param rect Rectangle for the scale

  \sa scaleRect(), activate()
 */
void AbstractPlotLayout::setScaleRect( int axis, const QRectF &rect )
{
    if ( !AbstractPlot::axisValid(axis) )
        axisRect[axis] = rect;
}

QRectF AbstractPlotLayout::getScaleRect( int axis ) const
{
    if ( !AbstractPlot::axisValid(axis) )
    {
        static QRectF dummyRect;
        return dummyRect;
    }
    return axisRect[axis];
}

/*!
  \brief Set the geometry for the canvas

  This method is intended to be used from derived layouts
  overloading activate()

  \sa canvasRect(), activate()
 */
void AbstractPlotLayout::setCanvasRect( const QRectF &rect ) {
    canvasRect = rect;
}

/*!
  \return Geometry for the canvas
  \sa activate(), invalidate()
*/
QRectF AbstractPlotLayout::getCanvasRect() const {
    return canvasRect;
}

/*!
 * \brief 将各组件的布局信息（位置和大小，geometry）清空。
 * \sa activate()
 */
void AbstractPlotLayout::invalidate()
{
    canvasRect = QRect();

    for ( int axis = 0; axis < AbstractPlot::axisCnt; axis++ )
        axisRect[axis] = QRect();
}

/*!
 * \return 期望的最小大小（minimum size hint）。
 * \param plot 坐标图控件。
 * \sa SimplePlot::minimumSizeHint()
 */
QSize AbstractPlotLayout::minimumSizeHint( const AbstractPlot *plot ) const
{
    class ScaleData
    {
    public:
        ScaleData()
        {
            w = h = minLeft = minRight = 0;
        }

        int w;
        int h;
        int minLeft;
        int minRight;
    } scaleData[AbstractPlot::axisCnt];

    int canvasBorder[AbstractPlot::axisCnt];

    int fw;
    plot->getCanvas()->getContentsMargins( &fw, NULL, NULL, NULL );

    int axis;
    for ( axis = 0; axis < AbstractPlot::axisCnt; axis++ )
    {
        if ( plot->isAxisEnabled( axis ) )
        {
            const AbstractAxis *scl = plot->getAxisWidget( axis );
            ScaleData &sd = scaleData[axis];

            const QSize hint = scl->minimumSizeHint();
            sd.w = hint.width();
            sd.h = hint.height();
            scl->getBorderDist( sd.minLeft, sd.minRight );
        }

        canvasBorder[axis] = fw + canvasMargin[axis] + 1;
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

                sd.h -= shiftBottom;
            }
            if ( ( sd.minLeft > canvasBorder[AbstractPlot::xTop] ) &&
                scaleData[AbstractPlot::xTop].h )
            {
                int shiftTop = sd.minRight - canvasBorder[AbstractPlot::xTop];

                sd.h -= shiftTop;
            }
        }
    }

    const QWidget *canvas = plot->getCanvas();

    int left, top, right, bottom;
    canvas->getContentsMargins( &left, &top, &right, &bottom );

    const QSize minCanvasSize = canvas->minimumSize();

    //
    int w = scaleData[AbstractPlot::yLeft].w + scaleData[AbstractPlot::yRight].w;
    int cw = qMax( scaleData[AbstractPlot::xBottom].w, scaleData[AbstractPlot::xTop].w )
        + left + 1 + right + 1;
    w += qMax( cw, minCanvasSize.width() );

    int h = scaleData[AbstractPlot::xBottom].h + scaleData[AbstractPlot::xTop].h;
    int ch = qMax( scaleData[AbstractPlot::yLeft].h, scaleData[AbstractPlot::yRight].h )
        + top + 1 + bottom + 1;
    h += qMax( ch, minCanvasSize.height() );

    return QSize( w, h );
}

/*!
 * \brief 重新计算各组件的布局信息。
 * \param plot 需要重新布局的Plot
 * \param plotRect 组件的显示区域。
 * \sa invalidate(), getAxisRect(), getCanvasRect()
 * \todo 当canvas有边框时，应该将坐标轴轴线和canvas内容对齐。但是，这里并没有考虑这一点，而是认为所有
 * 画布都是没有边框的。可以写一个函数alignScales来使坐标轴轴线和canvas内容对齐。
 */
void AbstractPlotLayout::activate(const AbstractPlot *plot, const QRectF &plotRect)
{
    invalidate();

    QRectF rect( plotRect );  // 分配Plot显示区域的剩余部分。

    // 首先从控件中获取布局相关参数，并将其存储起来。

    d_data->layoutData.init( plot );

    int dimAxes[AbstractPlot::axisCnt];//各个Axis的宽度（tick方向上）
    for(int i=AbstractPlot::yLeft; i<AbstractPlot::axisCnt; i++){
        dimAxes[i] = axisDimHint(plot, i);
    }

    QMargins canvasMargin;

    int topMargin = 0;
    int bottomMargin = 0;

    // 横轴和画布的间距取决于纵轴的BorderDist值。
    // 纵轴的 startBorderDist 对应顶部BorderDist，endBorderDist 对应底部BorderDist。
    if(plot->isAxisEnabled(AbstractPlot::yLeft)){
        int start, end;
        plot->getAxisWidget(AbstractPlot::yLeft)->getBorderDist(start, end);
        topMargin = qMax(topMargin, start);
        bottomMargin = qMax(bottomMargin, end);
    }

    if(plot->isAxisEnabled(AbstractPlot::yRight)){
        int start, end;
        plot->getAxisWidget(AbstractPlot::yRight)->getBorderDist(start, end);
        topMargin = qMax(topMargin, start);
        bottomMargin = qMax(bottomMargin, end);
    }

    // 如果横轴的BorderDist太长，则将纵轴的宽度拉长以适应
    // 横轴的 startBorderDist 对应左侧BorderDist，endBorderDist 对应右侧BorderDist。
    if(plot->isAxisEnabled(AbstractPlot::xTop)){
        int start, end;
        plot->getAxisWidget(AbstractPlot::xTop)->getBorderDist(start, end);

        dimAxes[AbstractPlot::yLeft] = qMax(dimAxes[AbstractPlot::yLeft], start);
        dimAxes[AbstractPlot::yRight] = qMax(dimAxes[AbstractPlot::yRight], end);
    }

    if(plot->isAxisEnabled(AbstractPlot::xBottom)){
        int start, end;
        plot->getAxisWidget(AbstractPlot::xBottom)->getBorderDist(start, end);

        dimAxes[AbstractPlot::yLeft] = qMax(dimAxes[AbstractPlot::yLeft], start);
        dimAxes[AbstractPlot::yRight] = qMax(dimAxes[AbstractPlot::yRight], end);
    }

    // 根据布局选项设置CanvasMargin。

    if(layoutOption == LayoutOption::MaximizeCanvasMargin){
        int maxMargin = qMax(topMargin, bottomMargin);
        canvasMargin = QMargins(maxMargin, maxMargin, maxMargin, maxMargin);
    }else{
        canvasMargin.setTop(topMargin);
        canvasMargin.setBottom(bottomMargin);
    }

    //设置canvas位置。

    canvasRect.setRect(
        rect.x() + dimAxes[AbstractPlot::yLeft] + canvasMargin.left(),
        rect.y() + dimAxes[AbstractPlot::xTop] + canvasMargin.top(),
        rect.width() - dimAxes[AbstractPlot::yRight] - dimAxes[AbstractPlot::yLeft] - canvasMargin.right() - canvasMargin.left(),
        rect.height() - dimAxes[AbstractPlot::xBottom] - dimAxes[AbstractPlot::xTop] - canvasMargin.top() - canvasMargin.bottom());

    //设定坐标轴控件的位置

    for ( int axis = 0; axis < AbstractPlot::axisCnt; axis++ ){
        if (!plot->isAxisEnabled(axis)) continue;

        if ( dimAxes[axis] ){
            const AbstractAxis* widget = plot->getAxisWidget(axis);
            QRectF &scaleRect = axisRect[axis];

            QPoint scaleRangeStart;
            int scaleRangeOffset;

            switch ( axis ){
            case AbstractPlot::yLeft:
                //scaleRangeStart对应坐标轴上最小数值的位置
                scaleRangeStart.setX(canvasRect.bottomLeft().x() - canvasMargin.left());
                scaleRangeStart.setY(canvasRect.bottomLeft().y());
                scaleRangeOffset = canvasRect.height();
                break;
            case AbstractPlot::yRight:
                //scaleRangeStart对应坐标轴上最小数值的位置
                scaleRangeStart.setX(canvasRect.bottomRight().x() + canvasMargin.right());
                scaleRangeStart.setY(canvasRect.bottomRight().y());
                scaleRangeOffset = canvasRect.height();
                break;
            case AbstractPlot::xBottom:
                //scaleRangeStart对应坐标轴上最小数值的位置
                scaleRangeStart.setX(canvasRect.bottomLeft().x());
                scaleRangeStart.setY(canvasRect.bottomLeft().y() + canvasMargin.bottom());
                scaleRangeOffset = canvasRect.width();
                break;
            case AbstractPlot::xTop:
                //scaleRangeStart对应坐标轴上最小数值的位置
                scaleRangeStart.setX(canvasRect.topLeft().x());
                scaleRangeStart.setY(canvasRect.topLeft().y() - canvasMargin.top());
                scaleRangeOffset = canvasRect.width();
                break;
            }
            scaleRect = QRectF(widget->rectHint(scaleRangeStart, scaleRangeOffset, dimAxes[axis]));
            scaleRect = scaleRect.normalized();
        }
    }
}

int AbstractPlotLayout::axisDimHint(const AbstractPlot *plot, int axisId) const
{
    if(plot->isAxisEnabled(axisId))
        return plot->getAxisWidget(axisId)->dimHint();
    else
        return 0;
}
