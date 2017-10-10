#include "qlineseries.h"
#include <qlineseries_p.h>
#include <linechartitem_p.h>
#include <chartdataset_p.h>
#include <charttheme_p.h>
#include <qchart_p.h>

SCHARTS_BEGIN_NAMESPACE

/*!
 * \class QLineSeries
 * \inmodule Qt Charts
 * \brief QLineSeries 类主要用于绘制折线图。
 *
 * 折线图以直线彼此连接的点序列的形式展示信息。
 *
 * \image examples_linechart.png
 *
 * 创建一个基本的折线图十分简单：
 * \code
 * QLineSeries* series = new QLineSeries();
 * series->append(0, 6);
 * series->append(2, 4);
 * ...
 * chart->addSeries(series);
 * \endcode
 */
/*!
    \qmltype LineSeries
    \instantiates QLineSeries
    \inqmlmodule QtCharts

    \inherits XYSeries

    \brief The LineSeries type is used for making line charts.

    The following QML shows how to create a simple line chart:
    \snippet qmlchart/qml/qmlchart/View2.qml 1
    \beginfloatleft
    \image examples_qmlchart2.png
    \endfloat
    \clearfloat
*/

/*!
    \fn virtual SeriesType QLineSeries::type() const
    \brief Returns type of series.
    \sa QAbstractSeries, SeriesType
*/

/*!
    \qmlproperty real LineSeries::width
    The width of the line. By default the width is 2.0.
*/

/*!
    \qmlproperty Qt::PenStyle LineSeries::style
    Controls the style of the line. Set to one of Qt.NoPen, Qt.SolidLine, Qt.DashLine, Qt.DotLine,
    Qt.DashDotLine or Qt.DashDotDotLine. Using Qt.CustomDashLine is not supported in the QML API.
    By default the style is Qt.SolidLine.
*/

/*!
    \qmlproperty Qt::PenCapStyle LineSeries::capStyle
    Controls the cap style of the line. Set to one of Qt.FlatCap, Qt.SquareCap or Qt.RoundCap. By
    default the cap style is Qt.SquareCap.
*/

/*!
    Constructs empty series object which is a child of \a parent.
    When series object is added to QChartView or QChart instance ownerships is transferred.
*/
QLineSeries::QLineSeries(QObject *parent)
    : QXYSeries(*new QLineSeriesPrivate(this), parent)
{

}

/*!
    \internal
*/
QLineSeries::QLineSeries(QLineSeriesPrivate &d, QObject *parent)
    : QXYSeries(d, parent)
{

}
/*!
    Destroys the object. Series added to QChartView or QChart instances are owned by those,
    and are deleted when mentioned object are destroyed.
*/
QLineSeries::~QLineSeries()
{
    Q_D(QLineSeries);
    if (d->m_chart)
        d->m_chart->removeSeries(this);
}

QAbstractSeries::SeriesType QLineSeries::type() const
{
    return QAbstractSeries::SeriesTypeLine;
}

/*
QDebug operator<< (QDebug debug, const QLineSeries series)
{
    Q_ASSERT(series.d_func()->m_x.size() == series.d_func()->m_y.size());
    int size = series.d_func()->m_x.size();
    for (int i=0; i<size; i++) {
        debug.nospace() << "(" << series.d_func()->m_x.at(i) << ','<< series.d_func()->m_y.at(i) << ") ";
    }
    return debug.space();
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QLineSeriesPrivate::QLineSeriesPrivate(QLineSeries *q)
    : QXYSeriesPrivate(q)
{

}

void QLineSeriesPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(QLineSeries);
    LineChartItem *line = new LineChartItem(q,parent);
    m_item.reset(line);
    QAbstractSeriesPrivate::initializeGraphics(parent);
}

void QLineSeriesPrivate::initializeTheme(int index, ChartTheme* theme, bool forced)
{
    Q_Q(QLineSeries);
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

#include "moc_qlineseries.cpp"

SCHARTS_END_NAMESPACE
