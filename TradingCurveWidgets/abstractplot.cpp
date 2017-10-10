#include "abstractplot.h"
#include "abstractplotlayout.h"
#include <QApplication>

AbstractPlot::AbstractPlot(QWidget *parent):
    QFrame(parent)
{
    //! \todo 在这里直接为所有Plot设置字体大小，是否合理？
    this->setStyleSheet(QString("font: 12px;"));
}

void AbstractPlot::updateLayout()
{
    AbstractPlotLayout* layout = getPlotLayout();

    layout->activate( this, contentsRect() );

    QRect scaleRect[AbstractPlot::axisCnt];
    for ( int axisId = 0; axisId < axisCnt; axisId++ )
        scaleRect[axisId] = layout->getScaleRect( axisId ).toRect();
    QRect canvasRect = layout->getCanvasRect().toRect();

    // 调整可见控件的大小，将其显示出来。

    for ( int axisId = 0; axisId < axisCnt; axisId++ )
    {
        if ( isAxisEnabled( axisId ) )
        {
            getAxisWidget( axisId )->setGeometry( scaleRect[axisId] );

#if 1
            if ( axisId == xBottom || axisId == xTop )
            {
                // do we need this code any longer ???

                QRegion r( scaleRect[axisId] );
                if ( isAxisEnabled( yLeft ) )
                    r = r.subtracted( QRegion( scaleRect[yLeft] ) );
                if ( isAxisEnabled( yRight ) )
                    r = r.subtracted( QRegion( scaleRect[yRight] ) );
                r.translate( -scaleRect[ axisId ].x(),
                    -scaleRect[axisId].y() );

                getAxisWidget( axisId )->setMask( r );
            }
#endif
            if ( !getAxisWidget( axisId )->isVisibleTo( this ) )
                getAxisWidget( axisId )->show();
        }
        else
            getAxisWidget( axisId )->hide();
    }

    canvas->setGeometry( canvasRect );
}

/*!
  Resize and update internal layout
  \param e Resize event
*/
void AbstractPlot::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    updateLayout();
}

/*!
 * \brief 添加对布局请求的处理。
 * \param event 事件。
 * \return 参见QFrame::event()。
 */
bool AbstractPlot::event(QEvent *event)
{
    bool ok = QFrame::event( event );
    switch ( event->type() ){
        case QEvent::LayoutRequest:
            updateLayout();
            break;
        case QEvent::PolishRequest:
            update();
            break;
        default:;
    }
    return ok;
}

/*!
 * \brief 事件过滤器。
 *
 * 坐标图（Plot）控件处理画布的以下事件：
 *
 * - QEvent::Resize
 * 画布的margin可能依赖于其大小。
 *
 * - QEvent::ContentsRectChange
 * 布局信息需要重新计算。
 *
 * \param onject 要过滤的对象。
 * \param event 事件
 * \return 参见QFrame::eventFilter()。
 * \sa updateLayout()
 */

bool AbstractPlot::eventFilter(QObject *object, QEvent *event)
{
    if ( object = canvas ){
        if( (event->type() == QEvent::Resize) || (event->type() == QEvent::ContentsRectChange)){
            updateLayout();
        }
    }
    return QFrame::eventFilter( object, event );
}

/*!
 * \brief 启用或者禁用自动重绘（auto replot）选项。
 *
 * 如果自动重绘选项被启用，则Plot将会通过操作成员函数来隐式更新（update implicitly）。因为这
 * 可能会比较耗时，所以建议将该选项关闭并且在必要时显式（explicity）调用replot()来重绘。
 *
 * 自动重绘选项默认是关闭（false）的，这意味着用户必须调用replot()来使变化可见。
 *
 * \param flag \c true 或是 \c false。默认为 \c true。
 * \sa replot()
 */
void AbstractPlot::enableAutoReplot(bool flag){
    autoReplot = flag;
}

/*!
 * \return 如果自动重绘（auto replot）选项已经启用，则返回true。
 * \sa enableAutoReplot()
 */
bool AbstractPlot::isAutoReplot() const {
    return autoReplot;
}

/*!
 * \brief 重绘坐标图的内容。
 *
 * 如果自动重绘（auto replot）选项没有启用（这是默认情况），或者曲线均和原始数据（raw data）相
 * 关联，则坐标图需要显示刷新（调用replot()）来使改变可见。
 *
 * \sa updateAxes(), setAutoReplot()
 */
void AbstractPlot::replot()
{
    // 绘制优化，当不可见时无需重绘。
    if(!isVisible()) return;

    bool doAutoReplot = isAutoReplot();
    enableAutoReplot( false );

    // updateAxes();

    // 坐标轴的脚标可能发生改变，这使得布局需要更新。这里需要在绘制
    // 之前处理这些，避免坐标轴（scales）和画布（canvas）不同步。

    QApplication::sendPostedEvents( this, QEvent::LayoutRequest );

    const bool ok = QMetaObject::invokeMethod(canvas, "replot", Qt::DirectConnection );

    if ( !ok ){
        // 当画布（canvas）对象没有replot函数的后备方案。
        canvas->update(canvas->contentsRect());
    }

    enableAutoReplot( doAutoReplot );
}

/*! \brief 如果isAutoReplot()是 \c true 则重绘。*/
void AbstractPlot::autoRefresh()
{
    if ( isAutoReplot() )
        replot();
}

void AbstractPlot::disableAxis(int axisId)
{
    enableAxis(axisId, false);
}

/*!
 * \brief 启用或禁用一个坐标轴控件。
 *
 * 当一个坐标轴控件被禁用时，仅仅表示它不将不会在屏幕中显示。曲线，标记（markers）可以和被禁用的坐标轴
 * 控件相关联。同时，屏幕坐标的映射转换功能仍然可以正常工作。默认情况下，只有 xBottom 和 yLeft 是被启
 * 用的。
 *
 * 注意，当需要启用的控件不存在时，创建默认的坐标轴对象，然后启用它。
 *
 * \param axisId 坐标轴序号。
 * \param tf \c true (启用) or \c false (禁用)
 */
void AbstractPlot::enableAxis(int axisId, bool tf)
{
    if(!axisValid(axisId)){
        return;
    }

    AbstractAxis* axis = getAxisWidget(axisId);
    bool needUpdate = false;

    if(axis != nullptr){
        needUpdate = tf ^ axis->isAxisEnabled();
        axis->enableAxis(tf);
    }else{
        // 需要启用坐标轴控件但是坐标轴控件不存在，则使用默认方式创建一个。
        if(tf){
            needUpdate = true;
            createDefaultAxis(axisId);
            getAxisWidget(axisId)->enableAxis(tf);
        }
    }

    if(needUpdate)
        updateLayout();
}

/*!
 * \brief 坐标轴控件是否存在。
 * \param axisId 坐标轴类型。
 * \return 如果存在（虽然不一定被启用），返回true；如果不存在，返回false。
 */
bool AbstractPlot::isAxisExist(int axisId) const {
    return (getAxisWidget(axisId) != nullptr);
}

bool AbstractPlot::isAxisEnabled(int axisId) const
{
    const AbstractAxis* axis = getAxisWidget(axisId);
    return axis ? axis->isAxisEnabled() : false;
}

bool AbstractPlot::axisValid(int axisId)
{
    return ( ( axisId >= AbstractPlot::yLeft ) && ( axisId < AbstractPlot::axisCnt ) );
}

void AbstractPlot::setAxesPalette(const QPalette &pal)
{
    setAxisPalette(yLeft, pal);
    setAxisPalette(yRight, pal);
    setAxisPalette(xTop, pal);
    setAxisPalette(xBottom, pal);
}

void AbstractPlot::setAxisPalette(int axisId, const QPalette &pal)
{
    AbstractAxis* axis = getAxisWidget(axisId);
    if(axis != nullptr)
        axis->setPalette(pal);
}

AbstractPlot::Axis AbstractPlot::axisType2Id(AbstractAxis::AxisType type)
{
    switch(type){
    case AbstractAxis::LeftScale:   return yLeft;
    case AbstractAxis::RightScale:  return yRight;
    case AbstractAxis::TopScale:    return xTop;
    case AbstractAxis::BottomScale: return xBottom;
    default: return axisCnt;
    }
}

QWidget *AbstractPlot::getCanvas() {
    return canvas;
}

/*!
 * \brief 设置坐标图控件的画布（canvas）。
 *
 * AbstractPlot把画布的方法作为元方法（meta methods）调用（见QMetaObject）。和使用传统的C++虚函数机制
 * 不同，这种方法允许使用canvas继承自QWidget或者QGLWidget的函数。
 *
 * 可以实现以下的的元方法（meta methods）：
 *
 * - replot()
 * 当画布没有提供replot()方法时，AbstractPlot将会调用update()。
 *
 * - borderPath()
 * 当需要剪辑画布内容时，需要边界路径（border path）的概念。当画布没有特殊形状时（例如圆角矩形），不重载
 * 这个方法也是可以的。
 *
 * 默认的画布是一个SimplePlotCanvas。
 *
 * \param canvas 画布控件。
 * \sa getCanvas()
 */
void AbstractPlot::setCanvas(QWidget *widget)
{
    if ( widget == canvas )
        return;

    delete canvas;
    canvas = widget;

    if ( canvas )
    {
        canvas->setParent( this );
        canvas->installEventFilter( this );

        if ( isVisible() )
            canvas->show();
    }
}

const QWidget *AbstractPlot::getCanvas() const {
    return canvas;
}
