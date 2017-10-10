#include <qsplineseries.h>
#include <qsplineseries_p.h>
#include <splinechartitem_p.h>
#include <chartdataset_p.h>
#include <charttheme_p.h>
#include <splineanimation_p.h>
#include <qchart_p.h>

/*!
 * \class QSplineSeries
 * \inmodule Qt Charts
 * \brief 该类存储数据以绘制一条曲线。
 *
 * QSplineSeries 不仅存储数据点位，还需要存储 QPainterPath 绘制曲线所需的分割控制点位（segment control points）。
 * 当数据变化时，将会自动计算控制点位。这些点位由特定算法生成，用来绘制一条正常的曲线。
 *
 * \image examples_splinechart.png
 *
 * 创建一个基本的曲线图非常简单：
 * \code
 * QSplineSeries* series = new QSplineSeries();
 * series->append(0, 6);
 * series->append(2, 4);
 * ...
 * chart->addSeries(series);
 * \endcode
 */
/*!
 * \qmltype SplineSeries
 * \instantiates QSplineSeries
 * \inqmlmodule QtCharts
 *
 * \inherits XYSeries
 *
 * \brief SplineSeries 类型主要用于绘制曲线图。
 *
 * The following QML shows how to create a simple spline chart:
 * \snippet qmlchart/qml/qmlchart/View3.qml 1
 * \beginfloatleft
 * \image examples_qmlchart3.png
 * \endfloat
 * \clearfloat
 */

/*!
    \fn QSeriesType QSplineSeries::type() const
    Returns the type of the series
*/

/*!
    \qmlproperty real SplineSeries::width
    The width of the line. By default the width is 2.0.
*/

/*!
    \qmlproperty Qt::PenStyle SplineSeries::style
    Controls the style of the line. Set to one of Qt.NoPen, Qt.SolidLine, Qt.DashLine, Qt.DotLine,
    Qt.DashDotLine or Qt.DashDotDotLine. Using Qt.CustomDashLine is not supported in the QML API.
    By default the style is Qt.SolidLine.
*/

/*!
    \qmlproperty Qt::PenCapStyle SplineSeries::capStyle
    Controls the cap style of the line. Set to one of Qt.FlatCap, Qt.SquareCap or Qt.RoundCap. By
    default the cap style is Qt.SquareCap.
*/

SCHARTS_BEGIN_NAMESPACE

/*!
    Constructs empty series object which is a child of \a parent.
    When series object is added to a QChart instance then the ownerships is transferred.
  */

QSplineSeries::QSplineSeries(QObject *parent)
    : QLineSeries(*new QSplineSeriesPrivate(this), parent)
{
}

/*!
  Destroys the object.
*/
QSplineSeries::~QSplineSeries()
{
    Q_D(QSplineSeries);
    if (d->m_chart)
        d->m_chart->removeSeries(this);
}

QAbstractSeries::SeriesType QSplineSeries::type() const
{
    return QAbstractSeries::SeriesTypeSpline;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QSplineSeriesPrivate::QSplineSeriesPrivate(QSplineSeries *q)
    : QLineSeriesPrivate(q)
{
}

void QSplineSeriesPrivate::initializeGraphics(QGraphicsItem* parent)
{
    Q_Q(QSplineSeries);
    SplineChartItem *spline = new SplineChartItem(q,parent);
    m_item.reset(spline);
    QAbstractSeriesPrivate::initializeGraphics(parent);
}

void QSplineSeriesPrivate::initializeTheme(int index, ChartTheme* theme, bool forced)
{
    Q_Q(QSplineSeries);
    const QList<QColor> colors = theme->seriesColors();

    if (forced || QChartPrivate::defaultPen() == m_pen) {
        QPen pen;
        pen.setColor(colors.at(index % colors.size()));
        pen.setWidthF(2);
        q->setPen(pen);
    }

    if (forced || QChartPrivate::defaultPen().color() == m_pointLabelsColor) {
        QColor color = theme->labelBrush().color();
        q->setPointLabelsColor(color);
    }
}

void QSplineSeriesPrivate::initializeAnimations(SCharts::QChart::AnimationOptions options,
                                                int duration, QEasingCurve &curve)
{
    SplineChartItem *item = static_cast<SplineChartItem *>(m_item.data());
    Q_ASSERT(item);
    if (item->animation())
        item->animation()->stopAndDestroyLater();

    if (options.testFlag(QChart::SeriesAnimations))
        item->setAnimation(new SplineAnimation(item, duration, curve));
    else
        item->setAnimation(0);
    QAbstractSeriesPrivate::initializeAnimations(options, duration, curve);
}

#include "moc_qsplineseries.cpp"
#include "moc_qsplineseries_p.cpp"

SCHARTS_END_NAMESPACE
