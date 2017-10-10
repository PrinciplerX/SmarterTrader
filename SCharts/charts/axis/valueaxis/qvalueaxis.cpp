#include <QValueAxis>
#include <qvalueaxis_p.h>
#include <chartvalueaxisx_p.h>
#include <chartvalueaxisy_p.h>
#include <abstractdomain_p.h>
#include <polarchartvalueaxisangular_p.h>
#include <polarchartvalueaxisradial_p.h>
#include <chartdataset_p.h>
#include <chartpresenter_p.h>
#include <charttheme_p.h>
#include <charthelpers_p.h>

SCHARTS_BEGIN_NAMESPACE
/*!
 * \class QValueAxis
 * \inmodule Qt Charts
 * \brief QValueAxis 类主要用来操作图表的坐标轴。
 *
 * ValueAxis 可以设置用来显示具有刻度的坐标轴线，网格和遮蔽（shades）。坐标轴的数值被绘制到刻度的位置上。
 *
 * 关于 QValueAxis 用法的示例代码。
 * \code
 * QChartView *chartView = new QChartView;
 * QLineSeries *series = new QLineSeries;
 * // ...
 * chartView->chart()->addSeries(series);
 *
 * QValueAxis *axisX = new QValueAxis;
 * axisX->setRange(10, 20.5);
 * axisX->setTickCount(10);
 * axisX->setLabelFormat("%.2f");
 * chartView->chart()->setAxisX(axisX, series);
 * \endcode
 */
/*!
    \qmltype ValueAxis
    \instantiates QValueAxis
    \inqmlmodule QtCharts

    \inherits AbstractAxis
    \brief The ValueAxis element is used for manipulating chart's axes

    ValueAxis can be setup to show axis line with tick marks, grid lines and shades.
    Values of axis are drawn to position of ticks

    Example about using ValueAxis:
    \code
        ChartView {
            ValueAxis {
                id: xAxis
                min: 0
                max: 10
            }
        // Add a few series...
        }
    \endcode
*/

/*!
  \property QValueAxis::min
  Defines the minimum value on the axis.
  When setting this property the max is adjusted if necessary, to ensure that the range remains valid.
*/
/*!
  \qmlproperty real ValueAxis::min
  Defines the minimum value on the axis.
  When setting this property the max is adjusted if necessary, to ensure that the range remains valid.
*/

/*!
  \property QValueAxis::max
  Defines the maximum value on the axis.
  When setting this property the min is adjusted if necessary, to ensure that the range remains valid.
*/
/*!
  \qmlproperty real ValueAxis::max
  Defines the maximum value on the axis.
  When setting this property the min is adjusted if necessary, to ensure that the range remains valid.
*/

/*!
  \property QValueAxis::tickCount
  Defines the number of ticks on the axis. This indicates how many grid lines are drawn on the
  chart. The default value is 5, and it can not be below 2.
*/
/*!
  \qmlproperty int ValueAxis::tickCount
   Defines the number of ticks on the axis. This indicates how many grid lines are drawn on the
  chart. The default value is 5, and it can not be below 2.
*/

/*!
  \property QValueAxis::minorTickCount
  Defines the number of minor ticks on the axis. This indicates how many grid lines are drawn
  between major ticks on the chart. Labels are not drawn for minor ticks. The default value is 0.
*/
/*!
  \qmlproperty int ValueAxis::minorTickCount
  Defines the number of minor ticks on the axis. This indicates how many grid lines are drawn
  between major ticks on the chart. Labels are not drawn for minor ticks. The default value is 0.
*/

/*!
 * \property QValueAxis::labelFormat
 * 定义坐标轴刻度标签的格式。
 * 支持的标识符有: d, i, o, x, X, f, F, e, E, g, G, 和 c。
 * 更多细节请参见 QString::sprintf() 。
 *
 * 如果 QChart::localizeNumbers 为 \c{true}，则支持的标识符局限为： d, e, E, f, g, G, 和 i。同时，只支持精度修饰
 * 词（modifier）。其余的格式取决于应用程序的默认 QLocale 设置。
 */
/*!
 * \qmlproperty real ValueAxis::labelFormat
 * 定义坐标轴刻度标签的格式。
 * 支持的标识符有: d, i, o, x, X, f, F, e, E, g, G, 和 c。
 * 更多细节请参见 QString::sprintf() 。
 *
 * 如果 QChart::localizeNumbers 为 \c{true}，则支持的标识符局限为： d, e, E, f, g, G, 和 i。同时，只支持精度修饰
 * 词（modifier）。其余的格式取决于应用程序的默认 QLocale 设置。
 */

/*!
  \fn void QValueAxis::minChanged(qreal min)
  Axis emits signal when \a min of axis has changed.
*/
/*!
  \qmlsignal ValueAxis::onMinChanged(real min)
  Axis emits signal when \a min of axis has changed.
*/

/*!
  \fn void QValueAxis::maxChanged(qreal max)
  Axis emits signal when \a max of axis has changed.
*/
/*!
  \qmlsignal ValueAxis::onMaxChanged(real max)
  Axis emits signal when \a max of axis has changed.
*/

/*!
  \fn void QValueAxis::tickCountChanged(int tickCount)
  Axis emits signal when \a tickCount of axis has changed.
*/
/*!
  \qmlsignal ValueAxis::tickCountChanged(int tickCount)
  Axis emits signal when \a tickCount of axis has changed.
*/

/*!
  \fn void QValueAxis::minorTickCountChanged(int minorTickCount)
  Axis emits signal when \a minorTickCount of axis has changed.
*/
/*!
  \qmlsignal ValueAxis::minorTickCountChanged(int minorTickCount)
  Axis emits signal when \a minorTickCount of axis has changed.
*/

/*!
  \fn void QValueAxis::rangeChanged(qreal min, qreal max)
  Axis emits signal when \a min or \a max of axis has changed.
*/

/*!
  \fn void QValueAxis::labelFormatChanged(const QString &format)
  Axis emits signal when \a format of axis labels has changed.
*/
/*!
  \qmlsignal ValueAxis::labelFormatChanged(const QString &format)
  Axis emits signal when \a format of axis labels has changed.
*/

/*!
    Constructs an axis object which is a child of \a parent.
*/
QValueAxis::QValueAxis(QObject *parent) :
    QAbstractAxis(*new QValueAxisPrivate(this), parent)
{

}

/*!
    \internal
*/
QValueAxis::QValueAxis(QValueAxisPrivate &d, QObject *parent)
    : QAbstractAxis(d, parent)
{

}

/*!
    Destroys the object
*/
QValueAxis::~QValueAxis()
{
    Q_D(QValueAxis);
    if (d->m_chart)
        d->m_chart->removeAxis(this);
}

void QValueAxis::setMin(qreal min)
{
    Q_D(QValueAxis);
    setRange(min, qMax(d->m_max, min));
}

qreal QValueAxis::min() const
{
    Q_D(const QValueAxis);
    return d->m_min;
}

void QValueAxis::setMax(qreal max)
{
    Q_D(QValueAxis);
    setRange(qMin(d->m_min, max), max);
}

qreal QValueAxis::max() const
{
    Q_D(const QValueAxis);
    return d->m_max;
}

/*!
  Sets range from \a min to \a max on the axis.
  If min is greater than max then this function returns without making any changes.
*/
void QValueAxis::setRange(qreal min, qreal max)
{
    Q_D(QValueAxis);
    d->setRange(min,max);
}

void QValueAxis::setTickCount(int count)
{
    Q_D(QValueAxis);
    if (d->m_tickCount != count && count >= 2) {
        d->m_tickCount = count;
        emit tickCountChanged(count);
    }
}

int QValueAxis::tickCount() const
{
    Q_D(const QValueAxis);
    return d->m_tickCount;
}

void QValueAxis::setMinorTickCount(int count)
{
    Q_D(QValueAxis);
    if (d->m_minorTickCount != count && count >= 0) {
        d->m_minorTickCount = count;
        emit minorTickCountChanged(count);
    }
}

int QValueAxis::minorTickCount() const
{
    Q_D(const QValueAxis);
    return d->m_minorTickCount;
}

void QValueAxis::setLabelFormat(const QString &format)
{
    Q_D(QValueAxis);
    d->m_format = format;
    emit labelFormatChanged(format);
}

QString QValueAxis::labelFormat() const
{
    Q_D(const QValueAxis);
    return d->m_format;
}

/*!
  Returns the type of the axis
*/
QAbstractAxis::AxisType QValueAxis::type() const
{
    return AxisTypeValue;
}

/*!
    This method modifies range and number of ticks on the axis to look "nice". Algorithm considers numbers that
    can be expressed as form of 1*10^n, 2* 10^n or 5*10^n as a nice numbers. These numbers are used for spacing the ticks.
    This method will modify the current range and number of ticks.
    \sa setRange(), setTickCount()
*/
void QValueAxis::applyNiceNumbers()
{
    Q_D(QValueAxis);
    if(d->m_applying) return;
    qreal min = d->m_min;
    qreal max = d->m_max;
    int ticks = d->m_tickCount;
    AbstractDomain::looseNiceNumbers(min,max,ticks);
    d->m_applying=true;
    d->setRange(min,max);
    setTickCount(ticks);
    d->m_applying=false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QValueAxisPrivate::QValueAxisPrivate(QValueAxis *q)
    : QAbstractAxisPrivate(q),
      m_min(0),
      m_max(0),
      m_tickCount(5),
      m_minorTickCount(0),
      m_format(QString::null),
      m_applying(false)
{

}

QValueAxisPrivate::~QValueAxisPrivate()
{

}

void QValueAxisPrivate::setMin(const QVariant &min)
{
    Q_Q(QValueAxis);
    bool ok;
    qreal value = min.toReal(&ok);
    if (ok)
        q->setMin(value);
}

void QValueAxisPrivate::setMax(const QVariant &max)
{
    Q_Q(QValueAxis);
    bool ok;
    qreal value = max.toReal(&ok);
    if (ok)
        q->setMax(value);
}

void QValueAxisPrivate::setRange(const QVariant &min, const QVariant &max)
{
    Q_Q(QValueAxis);
    bool ok1;
    bool ok2;
    qreal value1 = min.toReal(&ok1);
    qreal value2 = max.toReal(&ok2);
    if (ok1 && ok2)
        q->setRange(value1, value2);
}

void QValueAxisPrivate::setRange(qreal min, qreal max)
{
    Q_Q(QValueAxis);
    bool changed = false;

    if (min > max)
        return;

    if (!isValidValue(min, max)) {
        qWarning() << "Attempting to set invalid range for value axis: ["
                   << min << " - " << max << "]";
        return;
    }

    bool changeMin = false;
    if (m_min == 0 || min == 0)
        changeMin = !qFuzzyCompare(1 + m_min, 1 + min);
    else
        changeMin = !qFuzzyCompare(m_min, min);

    bool changeMax = false;
    if (m_max == 0 || max == 0)
        changeMax = !qFuzzyCompare(1 + m_max, 1 + max);
    else
        changeMax = !qFuzzyCompare(m_max, max);

    if (changeMin) {
        m_min = min;
        changed = true;
        emit q->minChanged(min);
    }

    if (changeMax) {
        m_max = max;
        changed = true;
        emit q->maxChanged(max);
    }

    if (changed) {
        emit rangeChanged(min,max);
        emit q->rangeChanged(min, max);
    }
}

void QValueAxisPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(QValueAxis);
    ChartAxisElement *axis(0);

    if (m_chart->chartType() == QChart::ChartTypeCartesian) {
        if (orientation() == Qt::Vertical)
            axis = new ChartValueAxisY(q,parent);
        if (orientation() == Qt::Horizontal)
            axis = new ChartValueAxisX(q,parent);
    }

    if (m_chart->chartType() == QChart::ChartTypePolar) {
        if (orientation() == Qt::Vertical)
            axis = new PolarChartValueAxisRadial(q, parent);
        if (orientation() == Qt::Horizontal)
            axis = new PolarChartValueAxisAngular(q, parent);
    }

    m_item.reset(axis);
    QAbstractAxisPrivate::initializeGraphics(parent);
}


void QValueAxisPrivate::initializeDomain(AbstractDomain *domain)
{
    if (orientation() == Qt::Vertical) {
        if (!qFuzzyIsNull(m_max - m_min))
            domain->setRangeY(m_min, m_max);
        else
            setRange(domain->minY(), domain->maxY());
    }
    if (orientation() == Qt::Horizontal) {
        if (!qFuzzyIsNull(m_max - m_min))
            domain->setRangeX(m_min, m_max);
        else
            setRange(domain->minX(), domain->maxX());
    }
}

#include "moc_qvalueaxis.cpp"
#include "moc_qvalueaxis_p.cpp"

SCHARTS_END_NAMESPACE
