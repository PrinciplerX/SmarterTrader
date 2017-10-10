#include <QChart>
#include <qchart_p.h>
#include <legendscroller_p.h>
#include <qlegend_p.h>
#include <chartbackground_p.h>
#include <QAbstractAxis>
#include <abstractchartlayout_p.h>
#include <charttheme_p.h>
#include <chartpresenter_p.h>
#include <chartdataset_p.h>
#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneResizeEvent>

SCHARTS_BEGIN_NAMESPACE

/*!
 * \enum QChart::ChartTheme
 *
 * 该枚举类型描述了图表使用的主题。
 *
 * \value ChartThemeLight 默认主题
 * \value ChartThemeBlueCerulean
 * \value ChartThemeDark
 * \value ChartThemeBrownSand
 * \value ChartThemeBlueNcs
 * \value ChartThemeHighContrast
 * \value ChartThemeBlueIcy
 * \value ChartThemeQt
 */

/*!
 * \enum QChart::AnimationOption
 *
 * 用于启用/禁用动画。默认为 NoAnimation 。
 *
 * \value NoAnimation
 * \value GridAxisAnimations
 * \value SeriesAnimations
 * \value AllAnimations
 */

/*!
 * \enum QChart::ChartType
 *
 * 该枚举类型描述了图表类型。
 *
 * \value ChartTypeUndefined
 * \value ChartTypeCartesian
 * \value ChartTypePolar
 */

/*!
 * \class QChart
 * \inmodule Qt Charts
 * \brief Qt Charts库的主体图表API。
 *
 * QChart 是一个可以显示在一个 QGraphicsScene 中的 QGraphicsWidget。它管理着不同类别序列的图形化表示和
 * 其他一些图标相关的对象如图例（legend）和坐标轴（axis）。如果仅仅想是在一个布局中显示一个图表，则应使用
 * 更合适的类 QChartView 而非 QChart。
 *
 * \sa QChartView, QPolarChart
 */

/*!
 * \property QChart::animationOptions
 * 图表的动画 \a 选项 。根据这个设置项，动画可以被启用/禁用。
 */

/*!
 * \property QChart::animationDuration
 * 图表动画的持续时间。
 */

/*!
 * \property QChart::animationEasingCurve
 * 图表动画的平滑曲线（easing curve）。
 */

/*!
 * \property QChart::backgroundVisible
 * 指定图表背景是否可见。
 * \sa setBackgroundBrush(), setBackgroundPen(), plotAreaBackgroundVisible
 */

/*!
 * \property QChart::dropShadowEnabled
 * 如果设置为true，背景的阴影效果会被启用。如果设置为false将会被禁用。注意阴影效果取决于主题，这意味着当切
 * 换至另一个主题时该设置可能会改变。
 */

/*!
 * \property QChart::backgroundRoundness
 * 图表背景拐角处圆弧的直径。
 */

/*!
 * \property QChart::margins
 * 绘图区域（坐标轴）和图表控件之间的空白（margin）宽度。
 */

/*!
 * \property QChart::theme
 * 主题是一个内置的UI风格相关设置的集合，可以应用于图表的所有显示元素，如颜色，画笔（pen），填充（brush）以
 * 及序列，坐标轴，标题，图例的字体。\l {Chart themes example} 展示了一个使用不同主题的例子。
 * \note 更改主题将会覆盖以前应用到序列上的所有自定义设置。
 */

/*!
 * \property QChart::title
 * 标题是一个图表的名称（标签，label）。它显示在图表的顶部位置。图表标题支持html格式。
 */

/*!
 * \property QChart::chartType
 * 图表类型表明一个图表是否是直角坐标图或者极坐标图。该属性为内部设置，不能更改（只读）。
 * \sa QPolarChart
 */

/*!
 * \property QChart::plotAreaBackgroundVisible
 * 指定图表绘图区域的背景是否可见。
 * \note 默认情况下绘图区域背景是不可见的，绘图区域使用图表的通用背景。
 * \sa setPlotAreaBackgroundBrush(), setPlotAreaBackgroundPen(), backgroundVisible
 */

/*!
 * \property QChart::localizeNumbers
 * \since QtCharts 2.0
 * 当设置为 \c{true} ，所有显示在不同序列和坐标轴标签上的数字都会使用使用本地属性设置的QLocale进行本地化处
 * 理。
 * 当设置为 \c{false} ，则总是使用“C”区域设置（locale）。
 * 默认为 \c{false}.
 * \note 该属性不会影响 QDateTimeAxis 坐标轴文字标签。该坐标轴总是使用本地区域设置的QLocale工作。
 *
 * \sa locale
 */

/*!
 * \property QChart::locale
 * \since QtCharts 2.0
 * 设置当 localizeNumbers 为\c{true}时用于格式化图标标签的区域设置。该属性还决定了用于格式化 QDateTimeAxis
 * 坐标轴标签的区域设置，而这和 localizeNumbers 属性无关。
 * 在图表被构建时，默认设置为应用程序默认区域设置。
 *
 * \sa localizeNumbers
 */

/*!
 * \property QChart::plotArea
 * 保存了用于绘制图表的矩形区域。该区域并不包含由 margins 定义的空白区域。
 */

/*!
 * \internal
 * 构建一个\a type 类型的图表对象，作为\a parent 的子对象。
 * 参数 \a wFlags 用来传递给 QGraphicsWidget 构造函数。
 * 该构造函数只由子类调用。
 */
QChart::QChart(QChart::ChartType type, QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags),
      d_ptr(new QChartPrivate(this, type))
{
    d_ptr->init();
}

/*!
 * 构建一个图表对象，作为\a parent 的子对象。
 * 参数 \a wFlags 用来传递给 QGraphicsWidget 构造函数。
 */
QChart::QChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags),
      d_ptr(new QChartPrivate(this, ChartTypeCartesian))
{
    d_ptr->init();
}

/*!
 * 析构图表对象以及它的子对象，例如加入到图表中的序列和坐标轴对象。
 */
QChart::~QChart()
{
    // 删除dataset，由它移除所有的序列和坐标轴。
    delete d_ptr->m_dataset;
    d_ptr->m_dataset = 0;
}

/*!
 * 把\a series 添加到图表中，并取得其所有权。
 *
 * \note 默认情况下，一个新添加的序列未被关联到任何坐标轴，这在包括在添加该序列前使用 createDefaultAxes() 为该图表
 * 创建的坐标轴。如果在图表显示之前没有坐标轴关联到新添加的序列上，序列则会像关联到一些刚好能把显示范围映射到图表的
 * 绘制区域的坐标轴一样绘制到图标上。在该图表同时显示其他合理关联到坐标轴的序列时可能会造成困惑。所以要确保要么总是
 * 在添加一个序列之后调用 createDefaultAxes() 或者显式地将序列关联到坐标轴上。
 *
 * \sa removeSeries(), removeAllSeries(), createDefaultAxes(), QAbstractSeries::attachAxis()
 */
void QChart::addSeries(QAbstractSeries *series)
{
    Q_ASSERT(series);
    d_ptr->m_dataset->addSeries(series);
}

/*!
 * 将\a series 从图表中移除。
 * 图表将会释放对该\a series 对象的所有权。
 *
 * \sa addSeries(), removeAllSeries()
 */
void QChart::removeSeries(QAbstractSeries *series)
{
    Q_ASSERT(series);
    d_ptr->m_dataset->removeSeries(series);
}

/*!
 * 移除并且删除所有之前添加到该图表中的序列对象。
 *
 * \sa addSeries(), removeSeries()
 */
void QChart::removeAllSeries()
{
    foreach (QAbstractSeries *s ,  d_ptr->m_dataset->series()){
        removeSeries(s);
        delete s;
    }
}

/*!
 Sets the \a brush that is used for painting the background of the chart area.
 */
void QChart::setBackgroundBrush(const QBrush &brush)
{
    d_ptr->m_presenter->setBackgroundBrush(brush);
}

/*!
 Gets the brush that is used for painting the background of the chart area.
 */
QBrush QChart::backgroundBrush() const
{
    return d_ptr->m_presenter->backgroundBrush();
}

/*!
 Sets the \a pen that is used for painting the background of the chart area.
 */
void QChart::setBackgroundPen(const QPen &pen)
{
    d_ptr->m_presenter->setBackgroundPen(pen);
}

/*!
 Gets the pen that is used for painting the background of the chart area.
 */
QPen QChart::backgroundPen() const
{
    return d_ptr->m_presenter->backgroundPen();
}

void QChart::setTitle(const QString &title)
{
    d_ptr->m_presenter->setTitle(title);
}

QString QChart::title() const
{
    return d_ptr->m_presenter->title();
}

/*!
 Sets the \a font that is used for drawing the chart title.
 */
void QChart::setTitleFont(const QFont &font)
{
    d_ptr->m_presenter->setTitleFont(font);
}

/*!
 Gets the font that is used for drawing the chart title.
 */
QFont QChart::titleFont() const
{
    return d_ptr->m_presenter->titleFont();
}

/*!
 Sets the \a brush used for drawing the title text.
 */
void QChart::setTitleBrush(const QBrush &brush)
{
    d_ptr->m_presenter->setTitleBrush(brush);
}

/*!
 Returns the brush used for drawing the title text.
 */
QBrush QChart::titleBrush() const
{
    return d_ptr->m_presenter->titleBrush();
}

void QChart::setTheme(QChart::ChartTheme theme)
{
    d_ptr->m_themeManager->setTheme(theme);
}

QChart::ChartTheme QChart::theme() const
{
    return d_ptr->m_themeManager->theme()->id();
}

/*!
 Zooms in the view by a factor of two.
 */
void QChart::zoomIn()
{
    d_ptr->zoomIn(2.0);
}

/*!
 Zooms in the view to a maximum level at which \a rect is still fully visible.
 \note This is not supported for polar charts.
 */
void QChart::zoomIn(const QRectF &rect)
{
    if (d_ptr->m_type == QChart::ChartTypePolar)
        return;
    d_ptr->zoomIn(rect);
}

/*!
 Zooms out the view by a factor of two.
 */
void QChart::zoomOut()
{
    d_ptr->zoomOut(2.0);
}

/*!
 Zooms in the view by a custom \a factor.

 A factor over 1.0 zooms the view in and factor between 0.0 and 1.0 zooms out.
 */
void QChart::zoom(qreal factor)
{
    if (qFuzzyCompare(factor, 0))
        return;

    if (qFuzzyCompare(factor, (qreal)1.0))
        return;

    if (factor < 0)
        return;

    if (factor > 1.0)
        d_ptr->zoomIn(factor);
    else
        d_ptr->zoomOut(1.0 / factor);
}


/*!
 Resets the series domains to what they were before any zoom method was called.
 Note that this will also reset any scrolls and explicit axis range settings done between
 the first zoom operation and calling this method. If no zoom operation has been
 done, this method does nothing.
 */
void QChart::zoomReset()
{
    d_ptr->zoomReset();
}

/*!
 Returns true if any series has a zoomed domain.
 */
bool QChart::isZoomed()
{
   return d_ptr->isZoomed();
}

/*!
 Returns a pointer to the horizontal axis attached to the specified \a series.
 If no \a series is specified, the first horizontal axis added to the chart is returned.

 \sa addAxis(), QAbstractSeries::attachAxis()
 */
QAbstractAxis *QChart::axisX(QAbstractSeries *series) const
{
    QList<QAbstractAxis *> axisList = axes(Qt::Horizontal, series);
    if (axisList.count())
        return axisList[0];
    return 0;
}

/*!
 Returns a pointer to the vertical axis attached to the specified \a series.
 If no \a series is specified, the first vertical axis added to the chart is returned.

 \sa addAxis(), QAbstractSeries::attachAxis()
 */
QAbstractAxis *QChart::axisY(QAbstractSeries *series) const
{
    QList<QAbstractAxis *> axisList = axes(Qt::Vertical, series);
    if (axisList.count())
        return axisList[0];
    return 0;
}

/*!
 Returns the axes attached to the \a series with \a orientation. If no \a series is provided,
 then all axes added to the chart with the specified orientation are returned.
 \sa addAxis(), createDefaultAxes()
 */
QList<QAbstractAxis *> QChart::axes(Qt::Orientations orientation, QAbstractSeries *series) const
{
    QList<QAbstractAxis *> result ;

    if (series) {
        foreach (QAbstractAxis *axis, series->attachedAxes()){
            if (orientation.testFlag(axis->orientation()))
                result << axis;
        }
    } else {
        foreach (QAbstractAxis *axis, d_ptr->m_dataset->axes()){
            if (orientation.testFlag(axis->orientation()) && !result.contains(axis))
                result << axis;
        }
    }

    return result;
}

/*!
 Creates axes for the chart based on the series that have already been added to the chart. Any axes previously added to
 the chart will be deleted.

 \note This function has to be called after all series have been added to the chart. The axes created by this function
 will NOT get automatically attached to any series added to the chart after this function has been called.
 A series with no axes attached will by default scale to utilize the entire plot area of the chart, which can be confusing
 if there are other series with properly attached axes also present.

 \table
     \header
         \li Series type
         \li X-axis
         \li Y-axis
     \row
         \li QXYSeries
         \li QValueAxis
         \li QValueAxis
     \row
         \li QBarSeries
         \li QBarCategoryAxis
         \li QValueAxis
     \row
         \li QPieSeries
         \li None
         \li None
 \endtable

 If there are several QXYSeries derived series added to the chart and no series of other types have been added, then only one pair of axes is created.
 If there are several series of different types added to the chart, then each series gets its own axes pair.

 The axes specific to the series can be later obtained from the chart by providing the series as the parameter for axes() function call.
 QPieSeries does not create any axes.

 \sa axisX(), axisY(), axes(), setAxisX(), setAxisY(), QAbstractSeries::attachAxis()
 */
void QChart::createDefaultAxes()
{
    d_ptr->m_dataset->createDefaultAxes();
}

/*!
 Returns the legend object of the chart. Ownership stays with the chart.
 */
QLegend *QChart::legend() const
{
    return d_ptr->m_legend;
}

void QChart::setMargins(const QMargins &margins)
{
    d_ptr->m_presenter->layout()->setMargins(margins);
}

QMargins QChart::margins() const
{
    return d_ptr->m_presenter->layout()->margins();
}

QChart::ChartType QChart::chartType() const
{
    return d_ptr->m_type;
}

QRectF QChart::plotArea() const
{
    return d_ptr->m_presenter->geometry();
}

/*!
    Sets the \a brush for the background of the plot area of the chart.

    \sa plotArea(), plotAreaBackgroundVisible, setPlotAreaBackgroundPen(), plotAreaBackgroundBrush()
 */
void QChart::setPlotAreaBackgroundBrush(const QBrush &brush)
{
    d_ptr->m_presenter->setPlotAreaBackgroundBrush(brush);
}

/*!
    Returns the brush for the background of the plot area of the chart.

    \sa plotArea(), plotAreaBackgroundVisible, plotAreaBackgroundPen(), setPlotAreaBackgroundBrush()
 */
QBrush QChart::plotAreaBackgroundBrush() const
{
    return d_ptr->m_presenter->plotAreaBackgroundBrush();
}

/*!
    Sets the \a pen for the background of the plot area of the chart.

    \sa plotArea(), plotAreaBackgroundVisible, setPlotAreaBackgroundBrush(), plotAreaBackgroundPen()
 */
void QChart::setPlotAreaBackgroundPen(const QPen &pen)
{
    d_ptr->m_presenter->setPlotAreaBackgroundPen(pen);
}

/*!
    Returns the pen for the background of the plot area of the chart.

    \sa plotArea(), plotAreaBackgroundVisible, plotAreaBackgroundBrush(), setPlotAreaBackgroundPen()
 */
QPen QChart::plotAreaBackgroundPen() const
{
    return d_ptr->m_presenter->plotAreaBackgroundPen();
}

void QChart::setPlotAreaBackgroundVisible(bool visible)
{
    d_ptr->m_presenter->setPlotAreaBackgroundVisible(visible);
}

bool QChart::isPlotAreaBackgroundVisible() const
{
    return d_ptr->m_presenter->isPlotAreaBackgroundVisible();
}

void QChart::setLocalizeNumbers(bool localize)
{
    d_ptr->m_presenter->setLocalizeNumbers(localize);
}

bool QChart::localizeNumbers() const
{
    return d_ptr->m_presenter->localizeNumbers();
}

void QChart::setLocale(const QLocale &locale)
{
    d_ptr->m_presenter->setLocale(locale);
}

QLocale QChart::locale() const
{
    return d_ptr->m_presenter->locale();
}

void QChart::setAnimationOptions(AnimationOptions options)
{
    d_ptr->m_presenter->setAnimationOptions(options);
}

QChart::AnimationOptions QChart::animationOptions() const
{
    return d_ptr->m_presenter->animationOptions();
}

void QChart::setAnimationDuration(int msecs)
{
    d_ptr->m_presenter->setAnimationDuration(msecs);
}

int QChart::animationDuration() const
{
    return d_ptr->m_presenter->animationDuration();
}

void QChart::setAnimationEasingCurve(const QEasingCurve &curve)
{
    d_ptr->m_presenter->setAnimationEasingCurve(curve);
}

QEasingCurve QChart::animationEasingCurve() const
{
    return d_ptr->m_presenter->animationEasingCurve();
}

/*!
    Scrolls the visible area of the chart by the distance defined in the \a dx and \a dy.

    For polar charts, \a dx indicates the angle along angular axis instead of distance.
 */
void QChart::scroll(qreal dx, qreal dy)
{
    d_ptr->scroll(dx,dy);
}

void QChart::setBackgroundVisible(bool visible)
{
    d_ptr->m_presenter->setBackgroundVisible(visible);
}

bool QChart::isBackgroundVisible() const
{
    return d_ptr->m_presenter->isBackgroundVisible();
}

void QChart::setDropShadowEnabled(bool enabled)
{
    d_ptr->m_presenter->setBackgroundDropShadowEnabled(enabled);
}

bool QChart::isDropShadowEnabled() const
{
    return d_ptr->m_presenter->isBackgroundDropShadowEnabled();
}

void QChart::setBackgroundRoundness(qreal diameter)
{
    d_ptr->m_presenter->setBackgroundRoundness(diameter);
}

qreal QChart::backgroundRoundness() const
{
    return d_ptr->m_presenter->backgroundRoundness();
}

/*!
  Returns all series that are added to the chart.

  \sa addSeries(), removeSeries(), removeAllSeries()
*/
QList<QAbstractSeries *> QChart::series() const
{
    return d_ptr->m_dataset->series();
}

/*!
 * 将坐标轴\a axis 添加到图表，并且将其作为一个对齐底部的坐标轴关联到序列对象\a series 上。图表获取坐标轴和
 * 序列对象的所有权。
 * 之前与\a series 关联的任何水平方向坐标轴都将被删除。
 * \sa axisX(), axisY(), setAxisY(), createDefaultAxes(), QAbstractSeries::attachAxis()
 */
void QChart::setAxisX(QAbstractAxis *axis ,QAbstractSeries *series)
{
    QList<QAbstractAxis*> list = axes(Qt::Horizontal, series);

    foreach (QAbstractAxis* a, list) {
        d_ptr->m_dataset->removeAxis(a);
        delete a;
    }

    if (!d_ptr->m_dataset->axes().contains(axis))
        d_ptr->m_dataset->addAxis(axis, Qt::AlignBottom);
    d_ptr->m_dataset->attachAxis(series, axis);
}

/*!
 * 将坐标轴\a axis 添加到图表，并且将其作为一个对齐左侧的坐标轴关联到序列对象\a series 上。图表获取坐标轴和
 * 序列对象的所有权。
 * 之前与\a series 关联的任何垂直方向坐标轴都将被删除。
 *
 * \sa axisX(), axisY(), setAxisX(), createDefaultAxes(), QAbstractSeries::attachAxis()
 */
void QChart::setAxisY(QAbstractAxis *axis ,QAbstractSeries *series)
{
    QList<QAbstractAxis*> list = axes(Qt::Vertical, series);

    foreach (QAbstractAxis* a, list) {
        d_ptr->m_dataset->removeAxis(a);
        delete a;
    }

    if (!d_ptr->m_dataset->axes().contains(axis))
        d_ptr->m_dataset->addAxis(axis, Qt::AlignLeft);
    d_ptr->m_dataset->attachAxis(series, axis);
}

/*!
  Adds the \a axis to the chart with \a alignment. The chart takes the ownership of the axis.

  \sa removeAxis(), createDefaultAxes(), QAbstractSeries::attachAxis()
*/
void QChart::addAxis(QAbstractAxis *axis, Qt::Alignment alignment)
{
    d_ptr->m_dataset->addAxis(axis, alignment);
}

/*!
  Removes the \a axis from the chart.
  The chart releases its ownership of the specified \a axis object.

  \sa addAxis(), createDefaultAxes(), QAbstractSeries::detachAxis()
*/
void QChart::removeAxis(QAbstractAxis *axis)
{
    d_ptr->m_dataset->removeAxis(axis);
}

/*!
  Returns the value in the \a series domain that corresponds to the \a position relative to chart widget.
*/
QPointF QChart::mapToValue(const QPointF &position, QAbstractSeries *series)
{
    return d_ptr->m_dataset->mapToValue(position, series);
}

/*!
  Returns the position on the chart widget that corresponds to the \a value in the \a series domain.
*/
QPointF QChart::mapToPosition(const QPointF &value, QAbstractSeries *series)
{
    return d_ptr->m_dataset->mapToPosition(value, series);
}

//===========================================================================================================
// 内部类型 QChartPrivate 的代码。
//===========================================================================================================

QChartPrivate::QChartPrivate(QChart *q, QChart::ChartType type):
    q_ptr(q),
    m_legend(0),
    m_dataset(new ChartDataSet(q)),
    m_presenter(new ChartPresenter(q, type)),
    m_themeManager(new ChartThemeManager(q)),
    m_type(type)
{
    // 当增减序列对象和坐标轴对象时，需要由 ChartPresenter 和 ChartThemeManager 分别处理。
    QObject::connect(m_dataset, SIGNAL(seriesAdded(QAbstractSeries*)), m_presenter, SLOT(handleSeriesAdded(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(seriesRemoved(QAbstractSeries*)), m_presenter, SLOT(handleSeriesRemoved(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(axisAdded(QAbstractAxis*)), m_presenter, SLOT(handleAxisAdded(QAbstractAxis*)));
    QObject::connect(m_dataset, SIGNAL(axisRemoved(QAbstractAxis*)), m_presenter, SLOT(handleAxisRemoved(QAbstractAxis*)));
    QObject::connect(m_dataset, SIGNAL(seriesAdded(QAbstractSeries*)), m_themeManager, SLOT(handleSeriesAdded(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(seriesRemoved(QAbstractSeries*)), m_themeManager, SLOT(handleSeriesRemoved(QAbstractSeries*)));
    QObject::connect(m_dataset, SIGNAL(axisAdded(QAbstractAxis*)), m_themeManager, SLOT(handleAxisAdded(QAbstractAxis*)));
    QObject::connect(m_dataset, SIGNAL(axisRemoved(QAbstractAxis*)), m_themeManager, SLOT(handleAxisRemoved(QAbstractAxis*)));
    QObject::connect(m_presenter, &ChartPresenter::plotAreaChanged, q, &QChart::plotAreaChanged);
}

QChartPrivate::~QChartPrivate()
{
    delete m_themeManager;
}

// Hackish solution to the problem of explicitly assigning the default pen/brush/font
// to a series or axis and having theme override it:
// Initialize pens, brushes, and fonts to something nobody is likely to ever use,
// so that default theme initialization will always set these properly.
QPen &QChartPrivate::defaultPen()
{
    static QPen defaultPen(QColor(1, 2, 0), 0.93247536);
    return defaultPen;
}

QBrush &QChartPrivate::defaultBrush()
{
    static QBrush defaultBrush(QColor(1, 2, 0), Qt::Dense7Pattern);
    return defaultBrush;
}

QFont &QChartPrivate::defaultFont()
{
    static bool defaultFontInitialized(false);
    static QFont defaultFont;
    if (!defaultFontInitialized) {
        defaultFont.setPointSizeF(8.34563465);
        defaultFontInitialized = true;
    }
    return defaultFont;
}

void QChartPrivate::init()
{
    m_legend = new LegendScroller(q_ptr);
    q_ptr->setTheme(QChart::ChartThemeLight);
    q_ptr->setLayout(m_presenter->layout());
}

void QChartPrivate::zoomIn(qreal factor)
{
    QRectF rect = m_presenter->geometry();
    rect.setWidth(rect.width() / factor);
    rect.setHeight(rect.height() / factor);
    rect.moveCenter(m_presenter->geometry().center());
    zoomIn(rect);
}

void QChartPrivate::zoomIn(const QRectF &rect)
{
    if (!rect.isValid())
        return;

    QRectF r = rect.normalized();
    const QRectF geometry = m_presenter->geometry();
    r.translate(-geometry.topLeft());

    if (!r.isValid())
        return;

    QPointF zoomPoint(r.center().x() / geometry.width(), r.center().y() / geometry.height());
    m_presenter->setState(ChartPresenter::ZoomInState,zoomPoint);
    m_dataset->zoomInDomain(r);
    m_presenter->setState(ChartPresenter::ShowState,QPointF());

}

void QChartPrivate::zoomReset()
{
    m_dataset->zoomResetDomain();
}

bool QChartPrivate::isZoomed()
{
    return m_dataset->isZoomedDomain();
}

void QChartPrivate::zoomOut(qreal factor)
{
    const QRectF geometry = m_presenter->geometry();

    QRectF r;
    r.setSize(geometry.size() / factor);
    r.moveCenter(QPointF(geometry.size().width()/2 ,geometry.size().height()/2));
    if (!r.isValid())
        return;

    QPointF zoomPoint(r.center().x() / geometry.width(), r.center().y() / geometry.height());
    m_presenter->setState(ChartPresenter::ZoomOutState,zoomPoint);
    m_dataset->zoomOutDomain(r);
    m_presenter->setState(ChartPresenter::ShowState,QPointF());
}

void QChartPrivate::scroll(qreal dx, qreal dy)
{
    if (dx < 0) m_presenter->setState(ChartPresenter::ScrollLeftState,QPointF());
    if (dx > 0) m_presenter->setState(ChartPresenter::ScrollRightState,QPointF());
    if (dy < 0) m_presenter->setState(ChartPresenter::ScrollUpState,QPointF());
    if (dy > 0) m_presenter->setState(ChartPresenter::ScrollDownState,QPointF());

    m_dataset->scrollDomain(dx, dy);
    m_presenter->setState(ChartPresenter::ShowState,QPointF());
}

#include "moc_qchart.cpp"

SCHARTS_END_NAMESPACE
