#include "../qabstractseries.h"
#include <qabstractseries_p.h>
#include <chartdataset_p.h>
#include <QChart>
#include <qchart_p.h>
#include <chartitem_p.h>
#include <xydomain_p.h>
#include <xlogydomain_p.h>
#include <logxydomain_p.h>
#include <logxlogydomain_p.h>

SCHARTS_BEGIN_NAMESPACE

/*!
 * \class QAbstractSeries
 * \inmodule Qt Charts
 * \brief Base class for all Qt Chart series.
 *
 * 通常使用由该类派生的特定序列类型而不是直接使用该基类。
 * \sa QXYSeries, QLineSeries, QSplineSeries, QScatterSeries, QAreaSeries, QAbstractBarSeries, QStackedBarSeries,
 * QPercentBarSeries, QPieSeries
 */

/*!
 * \qmltype AbstractSeries
 * \instantiates QAbstractSeries
 * \inqmlmodule QtCharts
 *
 * \brief 所有Qt Chart序列对象的通用基类。
 *
 * AbstractSeries是所有序列的基类。
 * 该类不能由用户实例化。
 */

/*!
 * \enum QAbstractSeries::SeriesType
 *
 * 序列对象的类型。
 *
 * \value SeriesTypeLine
 * \value SeriesTypeArea
 * \value SeriesTypeBar
 * \value SeriesTypeStackedBar
 * \value SeriesTypePercentBar
 * \value SeriesTypePie
 * \value SeriesTypeScatter
 * \value SeriesTypeSpline
 * \value SeriesTypeHorizontalBar
 * \value SeriesTypeHorizontalStackedBar
 * \value SeriesTypeHorizontalPercentBar
 * \value SeriesTypeBoxPlot
 */

/*!
 * \property QAbstractSeries::type
 * 序列对象的类型。
 */
/*!
 * \qmlproperty ChartView.SeriesType AbstractSeries::type
 * 序列对象的类型。
 */

/*!
 * \property QAbstractSeries::name
 * \brief 序列的名称。该名称会显示到序列的图例上，可以支持html格式。
 */
/*!
 * \qmlproperty string AbstractSeries::name
 * 序列的名称。该名称会显示到序列的图例上，可以支持html格式。
 */

/*!
 * \fn void QAbstractSeries::nameChanged()
 * 当序列名称改变时，发出该信号。
 */
/*!
 * \qmlsignal AbstractSeries::onNameChanged()
 * 当序列名称改变时，发出该信号。
 */

/*!
 * \property QAbstractSeries::visible
 * \brief 该序列对象是否可见；默认为true。
 */
/*!
 * \qmlproperty bool AbstractSeries::visible
 * 该序列对象是否可见；默认为true。
 */

/*!
 * \fn void QAbstractSeries::visibleChanged()
 * 当序列可见性改变时，发出该信号。
 */
/*!
 * \qmlsignal AbstractSeries::onVisibleChanged()
 * 当序列可见性改变时，发出该信号。
 */

/*!
 * \property QAbstractSeries::opacity
 * \brief 序列对象的不透明度。
 *
 * 默认情况下不透明度为 1.0。有效的数值范围是从 0.0 （完全透明） 到 1.0 （不透明）。
 */
/*!
 * \qmlproperty real AbstractSeries::opacity
 * 序列对象的不透明度。默认情况下不透明度为 1.0。
 * 有效的数值范围是从 0.0 （完全透明） 到 1.0 （不透明）。
 */

/*!
 * \fn void QAbstractSeries::opacityChanged()
 * 当序列对象的透明度发生变化时，发出该信号。
 */
/*!
 * \qmlsignal AbstractSeries::onOpacityChanged()
 * 当序列对象的透明度发生变化时，发出该信号。
 */

/*!
 * \property QAbstractSeries::useOpenGL
 * \brief 指定序列绘制是否使用OpenGL加速。
 *
 * OpenGL绘制序列仅支持 QLineSeries 和 QScatterSeries。用作 QAreaSeries 边界序列的线性序列无法使用OpenGL加速。当
 * 图表包含包含任何使用OpenGL绘制的序列时，将会在图表绘制区域的顶端创建一个透明的 QOpenGLWidget 。被加速的序列并非
 * 绘制在底层的 QGraphicsView ，而是绘制在创建的 QOpenGLWidget 上。
 *
 * 使用OpenGL加速序列绘制带来的性能提升取决于底层硬件，但是大多数情况下效果很明显。例如，在一台标准桌面计算机上，对
 * 于某个序列启用OpenGL加速通常能够在不降低帧数的情况下多渲染数百倍的点数。
 * 图表大小对帧数的影响更小。
 *
 * OpenGL对序列绘制的加速主要是用于需要快速绘制大量点数的情况。它为了提高效率做了优化，因此相比未被加速的序列而言缺
 * 少了许多特性：
 *
 * \list
 * \li 加速序列不支持序列动画。
 * \li 加速序列不支持抗锯齿。
 * \li 加速序列不支持点标签（Point labels）。
 * \li 加速序列将会忽视画笔风格和标记（marker）形状。
 *     仅支持实心线段和普通的散点。
 *     散点可能是圆形或者矩形，取决于底层的图形硬件和驱动。
 * \li 极坐标图不支持加速序列。
 * \li 加速序列不支持鼠标事件。
 * \li 当使用加速序列时不推荐启用图表阴影或者透明的图表背景颜色，因为这可能会显著降低帧率。
 * \endlist
 *
 * 这些附加的限制主要是基于加速序列在图表顶部的独立控件绘制的事实：
 *
 * \list
 * \li 如果你在图表顶部绘制任何包含了加速序列的图形元素I，加速序列将会在这些元素之上绘制。
 * \li 为了使 QOpenGLWidget 部分透明，它必须位于堆叠到其他控件的顶部。 这意味着你在使用加速序列时，不能有其他部分遮
 *     盖图表的空间。
 * \li 当 graphics scene 有多个关联的 graphics view 时，不支持加速序列。
 * \li 当图表没有充满整个 graphics view 或者有非默认的尺寸和位置（geometry）时，不支持加速序列。例如， 使用滚动条滚
 *     动视图或者给 graphics view 添加变换（transformation）导致加速序列绘制到相对图表的错误位置上。
 * \endlist
 *
 * 默认值为 \c{false}.
*/
/*!
 * \qmlproperty bool AbstractSeries::useOpenGL
 * 指定序列绘制是否使用OpenGL加速。
 *
 * OpenGL绘制序列仅支持 QLineSeries 和 QScatterSeries。用作 QAreaSeries 边界序列的线性序列无法使用OpenGL加速。当
 * 图表包含包含任何使用OpenGL绘制的序列时，将会在图表绘制区域的顶端创建一个透明的 QOpenGLWidget 。被加速的序列并非
 * 绘制在底层的 QGraphicsView ，而是绘制在创建的 QOpenGLWidget 上。
 *
 * 使用OpenGL加速序列绘制带来的性能提升取决于底层硬件，但是大多数情况下效果很明显。例如，在一台标准桌面计算机上，对
 * 于某个序列启用OpenGL加速通常能够在不降低帧数的情况下多渲染数百倍的点数。
 * 图表大小对帧数的影响更小。
 *
 * OpenGL对序列绘制的加速主要是用于需要快速绘制大量点数的情况。它为了提高效率做了优化，因此相比未被加速的序列而言缺
 * 少了许多特性：
 *
 * \list
 * \li 加速序列不支持序列动画。
 * \li 加速序列不支持抗锯齿。
 * \li 加速序列不支持点标签（Point labels）。
 * \li 加速序列将会忽视画笔风格和标记（marker）形状。
 *     仅支持实心线段和普通的散点。
 *     散点可能是圆形或者矩形，取决于底层的图形硬件和驱动。
 * \li 极坐标图不支持加速序列。
 * \li 加速序列不支持鼠标事件。
 * \li 当使用加速序列时不推荐启用图表阴影或者透明的图表背景颜色，因为这可能会显著降低帧率。
 * \endlist
 *
 * 默认值为 \c{false}.
 */

/*!
 * \fn void QAbstractSeries::useOpenGLChanged()
 * 当 useOpenGL 属性值发生变化时，发出该信号。
 */
/*!
 * \qmlsignal AbstractSeries::onUseOpenGLChanged()
 * 当 useOpenGL 属性值发生变化时，发出该信号。
 */

/*!
 * \internal
 * \brief 构建一个 QAbstractSeries 对象作为\a parent 的子对象。
 */
QAbstractSeries::QAbstractSeries(QAbstractSeriesPrivate &d, QObject *parent) :
    QObject(parent),
    d_ptr(&d)
{
}

/*!
 * \brief 图表序列的虚析构函数。
 */
QAbstractSeries::~QAbstractSeries()
{
    if (d_ptr->m_chart)
        qFatal("Series still bound to a chart when destroyed!");
}

void QAbstractSeries::setName(const QString &name)
{
    if (name != d_ptr->m_name) {
        d_ptr->m_name = name;
        emit nameChanged();
    }
}

QString QAbstractSeries::name() const
{
    return d_ptr->m_name;
}

void QAbstractSeries::setVisible(bool visible)
{
    if (visible != d_ptr->m_visible) {
        d_ptr->m_visible = visible;
        emit visibleChanged();
    }
}

bool QAbstractSeries::isVisible() const
{
    return d_ptr->m_visible;
}

qreal QAbstractSeries::opacity() const
{
    return d_ptr->m_opacity;
}

void QAbstractSeries::setOpacity(qreal opacity)
{
    if (opacity != d_ptr->m_opacity) {
        d_ptr->m_opacity = opacity;
        emit opacityChanged();
    }
}

void QAbstractSeries::setUseOpenGL(bool enable)
{
#ifdef QT_NO_OPENGL
    Q_UNUSED(enable)
#else
    bool polarChart = d_ptr->m_chart && d_ptr->m_chart->chartType() == QChart::ChartTypePolar;
    bool supportedSeries = (type() == SeriesTypeLine || type() == SeriesTypeScatter);
    if ((!enable || !d_ptr->m_blockOpenGL)
            && supportedSeries
            && enable != d_ptr->m_useOpenGL
            && (!enable || !polarChart)) {
        d_ptr->m_useOpenGL = enable;
        emit useOpenGLChanged();
    }
#endif
}

bool QAbstractSeries::useOpenGL() const
{
    return d_ptr->m_useOpenGL;
}

/*!
 * \brief 返回序列所属的图表对象。
 *
 * 当序列加入到图表中时自动设置，当序列从图表中移除时自动取消设置。
 */
QChart *QAbstractSeries::chart() const
{
    return d_ptr->m_chart;
}

/*!
 * \brief 将序列的可见性设为true。
 * \sa setVisible(), isVisible()
 */
void QAbstractSeries::show()
{
    setVisible(true);
}

/*!
 * \brief 将序列的可见性设为true。
 * \sa setVisible(), isVisible()
 */
void QAbstractSeries::hide()
{
    setVisible(false);
}

/*!
 * 将坐标轴\a axis 关联到该序列。
 * \return 如果坐标轴关联成功，返回true；否则返回false。
 * \note If multiple axes of same orientation are attached to same series,they will have same min/max ranges.
 * \sa QChart::addAxis(), QChart::createDefaultAxes()
 */
bool QAbstractSeries::attachAxis(QAbstractAxis* axis)
{
    if(d_ptr->m_chart) {
        return d_ptr->m_chart->d_ptr->m_dataset->attachAxis(this, axis);
    } else {
        qWarning()<<"Series not in the chart. Please addSeries to chart first.";
        return false;
    }
}

/*!
 * 将坐标轴\a axis 从序列对象分离。
 * \return 如果坐标轴分离成功，返回true；否则返回false。
 * \sa QChart::removeAxis()
 */
bool QAbstractSeries::detachAxis(QAbstractAxis* axis)
{
    if(d_ptr->m_chart) {
        return d_ptr->m_chart->d_ptr->m_dataset->detachAxis(this, axis);
    }
    else {
        qWarning()<<"Series not in the chart. Please addSeries to chart first.";
        return false;
    }
}

/*!
 * 返回一个关联到该序列对象的坐标轴列表。这些坐标轴通常是一个x坐标轴和一个y坐标轴，除了在使
 * 用 QPieSeries 的情形下未关联任何坐标轴。
 * \sa attachAxis(), detachAxis()
 */
QList<QAbstractAxis*> QAbstractSeries::attachedAxes()
{
    return d_ptr->m_axes;
}

//===========================================================================================================
// 内部类型 QAbstractSeriesPrivate 的代码。
//===========================================================================================================

QAbstractSeriesPrivate::QAbstractSeriesPrivate(QAbstractSeries *q)
    : q_ptr(q),
      m_chart(0),
      m_item(0),
      m_domain(new XYDomain()),
      m_visible(true),
      m_opacity(1.0),
      m_useOpenGL(false),
      m_blockOpenGL(false)
{
}

QAbstractSeriesPrivate::~QAbstractSeriesPrivate()
{
}

void QAbstractSeriesPrivate::setDomain(AbstractDomain* domain)
{
    Q_ASSERT(domain);
    if(m_domain.data()!=domain) {
        if(!m_item.isNull()) QObject::disconnect(m_domain.data(), SIGNAL(updated()), m_item.data(), SLOT(handleDomainUpdated()));
        m_domain.reset(domain);
        if(!m_item.isNull()) {
            QObject::connect(m_domain.data(), SIGNAL(updated()),m_item.data(), SLOT(handleDomainUpdated()));
            m_item->handleDomainUpdated();
        }
    }
}

void QAbstractSeriesPrivate::setPresenter(ChartPresenter *presenter)
{
    m_presenter = presenter;
}

ChartPresenter *QAbstractSeriesPrivate::presenter() const
{
    return m_presenter;
}

void QAbstractSeriesPrivate::initializeGraphics(QGraphicsItem* parent)
{
    Q_ASSERT(!m_item.isNull());
    Q_UNUSED(parent);
    QObject::connect(m_domain.data(), SIGNAL(updated()),m_item.data(), SLOT(handleDomainUpdated()));
}

void QAbstractSeriesPrivate::initializeAnimations(QChart::AnimationOptions options, int duration,
                                                  QEasingCurve &curve)
{
    Q_UNUSED(options);
    Q_UNUSED(duration);
    Q_UNUSED(curve);
}

bool QAbstractSeriesPrivate::reverseXAxis()
{
    bool reverseXAxis = false;
    if (m_axes.size() != 0 && !(m_chart->chartType() == QChart::ChartTypePolar)) {
        int i = 0;
        while (i < m_axes.size()) {
            if (m_axes.at(i)->orientation() == Qt::Horizontal && m_axes.at(i)->isReverse()) {
                reverseXAxis = true;
                break;
            }
            i++;
        }
    }

    return reverseXAxis;
}

bool QAbstractSeriesPrivate::reverseYAxis()
{
    bool reverseYAxis = false;
    if (m_axes.size() != 0 && !(m_chart->chartType() == QChart::ChartTypePolar)) {
        int i = 0;
        while (i < m_axes.size()) {
            if (m_axes.at(i)->orientation() == Qt::Vertical && m_axes.at(i)->isReverse()) {
                reverseYAxis = true;
                break;
            }
            i++;
        }
    }

    return reverseYAxis;
}

// This function can be used to explicitly block OpenGL use from some otherwise supported series,
// such as the line series used as edge series of an area series.
void QAbstractSeriesPrivate::setBlockOpenGL(bool enable)
{
    m_blockOpenGL = enable;
    if (enable)
        q_ptr->setUseOpenGL(false);
}

#include "moc_qabstractseries.cpp"
#include "moc_qabstractseries_p.cpp"

SCHARTS_END_NAMESPACE


