#include <QBarCategoryAxis>
#include <QCandlestickLegendMarker>
#include <QCandlestickSeries>
#include <QCandlestickSet>
#include <QValueAxis>
#include <QtCore/QDateTime>
#include <candlestickanimation_p.h>
#include <candlestickchartitem_p.h>
#include <chartdataset_p.h>
#include <charttheme_p.h>
#include <qcandlestickseries_p.h>
#include <qcandlestickset_p.h>
#include <qchart_p.h>

SCHARTS_BEGIN_NAMESPACE

/*!
 * \class QCandlestickSeries
 * \since 5.8
 * \inmodule Qt Charts
 * \brief Series for creating a candlestick chart.
 *
 * QCandlestickSeries represents a series of data shown as candlesticks. The purpose of this class
 * is to act as a container for single candlestick items. Each item is drawn to its own category
 * when using QBarCategoryAxis. QDateTimeAxis and QValueAxis can be used as alternatives to
 * QBarCategoryAxis. In this case, each candlestick item is drawn according to its timestamp value.
 *
 * \note ��һ�� QCandlestickSeries �����У�ʱ������ܳ����ظ�����ʹ�� QBarCategoryAxis ʱ��ʹ����ͬʱ���������
 * �еĵ�һ���Żᱻ���Ƴ��������һ��ͼ������˶�� QCandlestickSeries ʵ���������Բ�ͬ���ж�������ͬʱ���������
 * ��������ͳһ�����category���С���ʹ�� QValueAxis ���� QDateTimeAxisʱ��ʹ��ͬһ��ʱ��������߽����໥���ǡ�
 *
 * See the \l {Candlestick Chart Example} {candlestick chart example} to learn how to create
 * a candlestick chart.
 * \image examples_candlestickchart.png
 *
 * \sa QCandlestickSet, QBarCategoryAxis, QDateTimeAxis, QValueAxis
 */

/*!
    \qmltype CandlestickSeries
    \since 2.2
    \instantiates QCandlestickSeries
    \inqmlmodule QtCharts
    \inherits AbstractSeries
    \brief Series for creating a candlestick chart.

    CandlestickSeries represents a series of data shown as candlesticks. The purpose of this class
    is to act as a container for single candlestick items. Each item is drawn to its own category
    when using BarCategoryAxis. DateTimeAxis and ValueAxis can be used as an alternative to
    BarCategoryAxis. In this case each candlestick item is drawn according to its timestamp value.

    \note The timestamps must be unique within a CandlestickSeries. When using BarCategoryAxis, only
    the first one of the candlestick items sharing a timestamp is drawn. If the chart includes
    multiple instances of CandlestickSeries, items from different series sharing a timestamp are
    drawn to the same category. When using ValueAxis or DateTimeAxis, candlestick items sharing a
    timestamp will overlap each other.

    The following QML shows how to create a simple candlestick chart:
    \code
    import QtQuick 2.5
    import QtCharts 2.2

    ChartView {
        title: "Candlestick Series"
        width: 400
        height: 300

        CandlestickSeries {
            name: "Acme Ltd."
            increasingColor: "green"
            decreasingColor: "red"

            CandlestickSet { timestamp: 1435708800000; open: 690; high: 694; low: 599; close: 660 }
            CandlestickSet { timestamp: 1435795200000; open: 669; high: 669; low: 669; close: 669 }
            CandlestickSet { timestamp: 1436140800000; open: 485; high: 623; low: 485; close: 600 }
            CandlestickSet { timestamp: 1436227200000; open: 589; high: 615; low: 377; close: 569 }
            CandlestickSet { timestamp: 1436313600000; open: 464; high: 464; low: 254; close: 254 }
        }
    }
    \endcode

    \beginfloatleft
    \image examples_qmlcandlestick.png
    \endfloat
    \clearfloat

    \sa CandlestickSet, BarCategoryAxis, DateTimeAxis, ValueAxis
*/

/*!
    \property QCandlestickSeries::count
    \brief The count of sets in series.
*/

/*!
    \qmlproperty int CandlestickSeries::count
    The count of sets in series.
*/

/*!
    \property QCandlestickSeries::maximumColumnWidth
    \brief The maximum width of the candlestick items in pixels. Setting a negative value means
    there is no maximum width. All negative values are converted to -1.0.
*/

/*!
    \qmlproperty qreal CandlestickSeries::maximumColumnWidth
    \brief The maximum width of the candlestick items in pixels. Setting a negative value means
    there is no maximum width. All negative values are converted to -1.0.
*/

/*!
    \property QCandlestickSeries::minimumColumnWidth
    \brief The minimum width of the candlestick items in pixels. Setting a negative value means
    there is no minimum width. All negative values are converted to -1.0.
*/

/*!
    \qmlproperty qreal CandlestickSeries::minimumColumnWidth
    \brief The minimum width of the candlestick items in pixels. Setting a negative value means
    there is no minimum width. All negative values are converted to -1.0.
*/

/*!
    \property QCandlestickSeries::bodyWidth
    \brief The width of the candlestick items.

    The value signifies the relative width of the candlestick item inside its own slot, in the range
    0.0 to 1.0. Values outside this range are clamped to 0.0 or 1.0.
*/

/*!
    \qmlproperty qreal CandlestickSeries::bodyWidth
    \brief The width of the candlestick items.

    The value signifies the relative width of the candlestick item inside its own slot, in the range
    0.0 to 1.0. Values outside this range are clamped to 0.0 or 1.0.
*/

/*!
    \property QCandlestickSeries::bodyOutlineVisible
    \brief The visibility of the candlestick body outlines.
*/

/*!
    \qmlproperty bool CandlestickSeries::bodyOutlineVisible
    \brief The visibility of the candlestick body outlines.
*/

/*!
    \property QCandlestickSeries::capsWidth
    \brief The width of the caps.

    The value signifies the relative width of the caps inside its own candlestick, in the range 0.0
    to 1.0. Values outside this range are clamped to 0.0 or 1.0.
*/

/*!
    \qmlproperty qreal CandlestickSeries::capsWidth
    \brief The width of the caps.

    The value signifies the relative width of the caps inside its own candlestick, in the range 0.0
    to 1.0. Values outside this range are clamped to 0.0 or 1.0.
*/

/*!
    \property QCandlestickSeries::capsVisible
    \brief The visibility of the caps.
*/

/*!
    \qmlproperty bool CandlestickSeries::capsVisible
    \brief The visibility of the caps.
*/

/*!
    \property QCandlestickSeries::increasingColor
    \brief The color of the increasing candlestick item body. Candlestick is \e increasing when its
    close value is higher than the open value. By default this property is set to brush color.
    Default color is used also when the property is set to an invalid color value.
*/

/*!
    \qmlproperty QColor CandlestickSeries::increasingColor
    \brief The color of the increasing candlestick item body. Candlestick is \e increasing when its
    close value is higher than the open value. By default this property is set to brush color.
    Default color is used also when the property is set to an invalid color value.
*/

/*!
    \property QCandlestickSeries::decreasingColor
    \brief The color of the decreasing candlestick item body. Candlestick is \e decreasing when its
    open value is higher than the close value. By default this property is set to brush color with
    alpha channel set to 128. Default color is used also when the property is set to an invalid
    color value.
*/

/*!
    \qmlproperty QColor CandlestickSeries::decreasingColor
    \brief The color of the decreasing candlestick item body. Candlestick is \e decreasing when its
    open value is higher than the close value. By default this property is set to brush color with
    alpha channel set to 128. Default color is used also when the property is set to an invalid
    color value.
*/

/*!
    \property QCandlestickSeries::brush
    \brief The brush of the candlestick items.
*/

/*!
    \property QCandlestickSeries::pen
    \brief The pen of the candlestick items.
*/

/*!
    \qmlproperty QString CandlestickSeries::brushFilename
    \brief The name of the file used as a brush for the series.
*/

/*!
    \fn void QCandlestickSeries::clicked(QCandlestickSet *set)
    \brief Emitted when a \a set is clicked (pressed and released) on the chart.
*/

/*!
    \qmlsignal CandlestickSeries::clicked(CandlestickSet set)
    \brief Emitted when a \a set is clicked (pressed and released) on the chart.

    The corresponding signal handler is \c {onClicked}.
*/

/*!
    \fn void QCandlestickSeries::hovered(bool status, QCandlestickSet *set)
    \brief Emitted when there is change in hover \a status over the \a set.
*/

/*!
    \qmlsignal CandlestickSeries::hovered(bool status, CandlestickSet set)
    \brief Emitted when there is change in hover \a status over the \a set.

    The corresponding signal handler is \c {onHovered}.
*/

/*!
    \fn void QCandlestickSeries::pressed(QCandlestickSet *set)
    \brief Emitted when a \a set is pressed on the chart.
*/

/*!
    \qmlsignal CandlestickSeries::pressed(CandlestickSet set)
    \brief Emitted when a \a set is pressed on the chart.

    The corresponding signal handler is \c {onPressed}.
*/

/*!
    \fn void QCandlestickSeries::released(QCandlestickSet *set)
    \brief Emitted when a \a set is released on the chart.
*/

/*!
    \qmlsignal CandlestickSeries::released(CandlestickSet set)
    \brief Emitted when a \a set is released on the chart.

    The corresponding signal handler is \c {onReleased}.
*/

/*!
    \fn void QCandlestickSeries::doubleClicked(QCandlestickSet *set)
    \brief Emitted when a \a set is double-clicked on the chart.
*/

/*!
    \qmlsignal CandlestickSeries::doubleClicked(CandlestickSet set)
    \brief Emitted when a \a set is double-clicked on the chart.

    The corresponding signal handler is \c {onDoubleClicked}.
*/

/*!
    \fn void QCandlestickSeries::candlestickSetsAdded(const QList<QCandlestickSet *> &sets)
    \brief Emitted when new \a sets are added to the series.
*/

/*!
    \qmlsignal CandlestickSeries::candlestickSetsAdded(list<CandlestickSet> sets)
    \brief Emitted when new \a sets are added to the series.

    The corresponding signal handler is \c {onCandlestickSetsAdded}.
*/

/*!
    \fn void QCandlestickSeries::candlestickSetsRemoved(const QList<QCandlestickSet *> &sets)
    \brief Emitted when \a sets are removed from the series.
*/

/*!
    \qmlsignal CandlestickSeries::candlestickSetsRemoved(list<CandlestickSet> sets)
    \brief Emitted when \a sets are removed from the series.

    The corresponding signal handler is \c {onCandlestickSetsRemoved}.
*/

/*!
    \fn void QCandlestickSeries::countChanged()
    \brief Emitted when there is a change in the count of candlestick items in the series.
    \sa count
*/

/*!
    \qmlsignal CandlestickSeries::countChanged()
    \brief Emitted when there is a change in the count of candlestick items in the series.
    \sa count

    The corresponding signal handler is \c {onCountChanged}.
*/

/*!
    \fn void QCandlestickSeries::maximumColumnWidthChanged()
    \brief Emitted when there is a change in the maximum column width of candlestick items.
    \sa maximumColumnWidth
*/

/*!
    \qmlsignal CandlestickSeries::maximumColumnWidthChanged()
    \brief Emitted when there is a change in the maximum column width of candlestick items.
    \sa maximumColumnWidth

    The corresponding signal handler is \c {onMaximumColumnWidthChanged}.
*/

/*!
    \fn void QCandlestickSeries::minimumColumnWidthChanged()
    \brief Emitted when there is a change in the minimum column width of candlestick items.
    \sa minimumColumnWidth
*/

/*!
    \qmlsignal CandlestickSeries::minimumColumnWidthChanged()
    \brief Emitted when there is a change in the minimum column width of candlestick items.
    \sa minimumColumnWidth

    The corresponding signal handler is \c {onMinimumColumnWidthChanged}.
*/

/*!
    \fn void QCandlestickSeries::bodyWidthChanged()
    \brief Emitted when the candlestick item width is changed.
    \sa bodyWidth
*/

/*!
    \qmlsignal CandlestickSeries::bodyWidthChanged()
    \brief Emitted when the candlestick item width is changed.
    \sa bodyWidth

    The corresponding signal handler is \c {onBodyWidthChanged}.
*/

/*!
    \fn void QCandlestickSeries::bodyOutlineVisibilityChanged()
    \brief Emitted when the visibility of the candlestick item body outline is changed.
    \sa bodyOutlineVisible
*/

/*!
    \qmlsignal CandlestickSeries::bodyOutlineVisibilityChanged()
    \brief Emitted when the visibility of the candlestick item body outline is changed.
    \sa bodyOutlineVisible

    The corresponding signal handler is \c {onBodyOutlineVisibilityChanged}.
*/

/*!
    \fn void QCandlestickSeries::capsWidthChanged()
    \brief Emitted when the candlestick item caps width is changed.
    \sa capsWidth
*/

/*!
    \qmlsignal CandlestickSeries::capsWidthChanged()
    \brief Emitted when the candlestick item caps width is changed.
    \sa capsWidth

    The corresponding signal handler is \c {onCapsWidthChanged}.
*/

/*!
    \fn void QCandlestickSeries::capsVisibilityChanged()
    \brief Emitted when the visibility of the candlestick item caps is changed.
    \sa capsVisible
*/

/*!
    \qmlsignal CandlestickSeries::capsVisibilityChanged()
    \brief Emitted when the visibility of the candlestick item caps is changed.
    \sa capsVisible

    The corresponding signal handler is \c {onCapsVisibilityChanged}.
*/

/*!
    \fn void QCandlestickSeries::increasingColorChanged()
    \brief Emitted when the candlestick item increasing color is changed.
    \sa increasingColor
*/

/*!
    \qmlsignal CandlestickSeries::increasingColorChanged()
    \brief Emitted when the candlestick item increasing color is changed.
    \sa increasingColor

    The corresponding signal handler is \c {onIncreasingColorChanged}.
*/

/*!
    \fn void QCandlestickSeries::decreasingColorChanged()
    \brief Emitted when the candlestick item decreasing color is changed.
    \sa decreasingColor
*/

/*!
    \qmlsignal CandlestickSeries::decreasingColorChanged()
    \brief Emitted when the candlestick item decreasing color is changed.
    \sa decreasingColor

    The corresponding signal handler is \c {onDecreasingColorChanged}.
*/

/*!
    \fn void QCandlestickSeries::brushChanged()
    \brief Emitted when the candlestick item brush is changed.
    \sa brush
*/

/*!
    \fn void QCandlestickSeries::penChanged()
    \brief Emitted when the candlestick item pen is changed.
    \sa pen
*/

/*!
    Constructs an empty QCandlestickSeries. The \a parent is optional.
*/
QCandlestickSeries::QCandlestickSeries(QObject *parent)
    : QAbstractSeries(*new QCandlestickSeriesPrivate(this), parent)
{
}

/*!
    Destroys the series. Removes the series from the chart.
*/
QCandlestickSeries::~QCandlestickSeries()
{
    Q_D(QCandlestickSeries);
    if (d->m_chart)
        d->m_chart->removeSeries(this);
}

/*!
    Adds a single set to the series. Takes ownership of the \a set. If the set is \e null or is
    already in the series, it won't be appended.
    Returns \c true if appending succeeded, \c false otherwise.
*/
bool QCandlestickSeries::append(QCandlestickSet *set)
{
    QList<QCandlestickSet *> sets;
    sets.append(set);

    return append(sets);
}

/*!
    Removes a single set from the series.
    Returns \c true if the \a set is successfully deleted, \c false otherwise.
*/
bool QCandlestickSeries::remove(QCandlestickSet *set)
{
    QList<QCandlestickSet *> sets;
    sets.append(set);

    return remove(sets);
}

/*!
    Adds a list of sets to the series. Takes ownership of the \a sets. If any of the sets are
    \e null, already appended to the series, or the list contains duplicated sets, nothing is
    appended.
    Returns \c true if all sets were appended successfully, \c false otherwise.
*/
bool QCandlestickSeries::append(const QList<QCandlestickSet *> &sets)
{
    Q_D(QCandlestickSeries);

    bool success = d->append(sets);
    if (success) {
        emit candlestickSetsAdded(sets);
        emit countChanged();
    }

    return success;
}

/*!
    Removes a list of sets from the series. If any of the \a sets are \e null, already removed from
    the series, or the list contains duplicated sets, nothing is removed.
    Returns \c true if all sets were removed successfully, \c false otherwise.
*/
bool QCandlestickSeries::remove(const QList<QCandlestickSet *> &sets)
{
    Q_D(QCandlestickSeries);

    bool success = d->remove(sets);
    if (success) {
        emit candlestickSetsRemoved(sets);
        emit countChanged();
        foreach (QCandlestickSet *set, sets)
            set->deleteLater();
    }

    return success;
}

/*!
    Inserts a set to the series at \a index position. Takes ownership of the \a set. If the set is
    \e null or already in the series, it won't be appended.
    Returns \c true if inserting succeeded, \c false otherwise.
*/
bool QCandlestickSeries::insert(int index, QCandlestickSet *set)
{
    Q_D(QCandlestickSeries);

    bool success = d->insert(index, set);
    if (success) {
        QList<QCandlestickSet *> sets;
        sets.append(set);
        emit candlestickSetsAdded(sets);
        emit countChanged();
    }

    return success;
}

/*!
    Takes a single \a set from the series. Does not delete the set object.
    Returns \c true if take was successful, \c false otherwise.
    \note The series remains as the set's parent object. You must set the parent object to take full
    ownership.
*/
bool QCandlestickSeries::take(QCandlestickSet *set)
{
    Q_D(QCandlestickSeries);

    QList<QCandlestickSet *> sets;
    sets.append(set);

    bool success = d->remove(sets);
    if (success) {
        emit candlestickSetsRemoved(sets);
        emit countChanged();
    }

    return success;
}

/*!
    Removes all sets from the series, and deletes them.
*/
void QCandlestickSeries::clear()
{
    Q_D(QCandlestickSeries);

    QList<QCandlestickSet *> sets = this->sets();

    bool success = d->remove(sets);
    if (success) {
        emit candlestickSetsRemoved(sets);
        emit countChanged();
        foreach (QCandlestickSet *set, sets)
            set->deleteLater();
    }
}

/*!
    Returns the list of sets in the series. Ownership of the sets is unchanged.
 */
QList<QCandlestickSet *> QCandlestickSeries::sets() const
{
    Q_D(const QCandlestickSeries);

    return d->m_sets;
}

/*!
    Returns the number of the sets in the series.
*/
int QCandlestickSeries::count() const
{
    return sets().count();
}

/*!
    Returns the type of the series (QAbstractSeries::SeriesTypeCandlestick).
*/
QAbstractSeries::SeriesType QCandlestickSeries::type() const
{
    return QAbstractSeries::SeriesTypeCandlestick;
}

void QCandlestickSeries::setMaximumColumnWidth(qreal maximumColumnWidth)
{
    Q_D(QCandlestickSeries);

    if (maximumColumnWidth < 0.0 && maximumColumnWidth != -1.0)
        maximumColumnWidth = -1.0;

    if (d->m_maximumColumnWidth == maximumColumnWidth)
        return;

    d->m_maximumColumnWidth = maximumColumnWidth;

    emit d->updatedLayout();
    emit maximumColumnWidthChanged();
}

qreal QCandlestickSeries::maximumColumnWidth() const
{
    Q_D(const QCandlestickSeries);

    return d->m_maximumColumnWidth;
}

void QCandlestickSeries::setMinimumColumnWidth(qreal minimumColumnWidth)
{
    Q_D(QCandlestickSeries);

    if (minimumColumnWidth < 0.0 && minimumColumnWidth != -1.0)
        minimumColumnWidth = -1.0;

    if (d->m_minimumColumnWidth == minimumColumnWidth)
        return;

    d->m_minimumColumnWidth = minimumColumnWidth;

    d->updatedLayout();
    emit minimumColumnWidthChanged();
}

qreal QCandlestickSeries::minimumColumnWidth() const
{
    Q_D(const QCandlestickSeries);

    return d->m_minimumColumnWidth;
}

void QCandlestickSeries::setBodyWidth(qreal bodyWidth)
{
    Q_D(QCandlestickSeries);

    if (bodyWidth < 0.0)
        bodyWidth = 0.0;
    else if (bodyWidth > 1.0)
        bodyWidth = 1.0;

    if (d->m_bodyWidth == bodyWidth)
        return;

    d->m_bodyWidth = bodyWidth;

    emit d->updatedLayout();
    emit bodyWidthChanged();
}

qreal QCandlestickSeries::bodyWidth() const
{
    Q_D(const QCandlestickSeries);

    return d->m_bodyWidth;
}

void QCandlestickSeries::setBodyOutlineVisible(bool bodyOutlineVisible)
{
    Q_D(QCandlestickSeries);

    if (d->m_bodyOutlineVisible == bodyOutlineVisible)
        return;

    d->m_bodyOutlineVisible = bodyOutlineVisible;

    emit d->updated();
    emit bodyOutlineVisibilityChanged();
}

bool QCandlestickSeries::bodyOutlineVisible() const
{
    Q_D(const QCandlestickSeries);

    return d->m_bodyOutlineVisible;
}

void QCandlestickSeries::setCapsWidth(qreal capsWidth)
{
    Q_D(QCandlestickSeries);

    if (capsWidth < 0.0)
        capsWidth = 0.0;
    else if (capsWidth > 1.0)
        capsWidth = 1.0;

    if (d->m_capsWidth == capsWidth)
        return;

    d->m_capsWidth = capsWidth;

    emit d->updatedLayout();
    emit capsWidthChanged();
}

qreal QCandlestickSeries::capsWidth() const
{
    Q_D(const QCandlestickSeries);

    return d->m_capsWidth;
}

void QCandlestickSeries::setCapsVisible(bool capsVisible)
{
    Q_D(QCandlestickSeries);

    if (d->m_capsVisible == capsVisible)
        return;

    d->m_capsVisible = capsVisible;

    emit d->updated();
    emit capsVisibilityChanged();
}

bool QCandlestickSeries::capsVisible() const
{
    Q_D(const QCandlestickSeries);

    return d->m_capsVisible;
}

void QCandlestickSeries::setIncreasingColor(const QColor &increasingColor)
{
    Q_D(QCandlestickSeries);

    QColor color;
    if (increasingColor.isValid()) {
        color = increasingColor;
        d->m_customIncreasingColor = true;
    } else {
        color = d->m_brush.color();
        color.setAlpha(128);
        d->m_customIncreasingColor = false;
    }

    if (d->m_increasingColor == color)
        return;

    d->m_increasingColor = color;

    emit d->updated();
    emit increasingColorChanged();
}

QColor QCandlestickSeries::increasingColor() const
{
    Q_D(const QCandlestickSeries);

    return d->m_increasingColor;
}

void QCandlestickSeries::setDecreasingColor(const QColor &decreasingColor)
{
    Q_D(QCandlestickSeries);

    QColor color;
    if (decreasingColor.isValid()) {
        color = decreasingColor;
        d->m_customDecreasingColor = true;
    } else {
        color = d->m_brush.color();
        d->m_customDecreasingColor = false;
    }

    if (d->m_decreasingColor == color)
        return;

    d->m_decreasingColor = color;

    emit d->updated();
    emit decreasingColorChanged();
}

QColor QCandlestickSeries::decreasingColor() const
{
    Q_D(const QCandlestickSeries);

    return d->m_decreasingColor;
}

void QCandlestickSeries::setBrush(const QBrush &brush)
{
    Q_D(QCandlestickSeries);

    if (d->m_brush == brush)
        return;

    d->m_brush = brush;
    if (!d->m_customIncreasingColor) {
        QColor color = d->m_brush.color();
        color.setAlpha(128);
        if (d->m_increasingColor != color) {
            d->m_increasingColor = color;
            emit increasingColorChanged();
        }
    }
    if (!d->m_customDecreasingColor && d->m_decreasingColor != d->m_brush.color()) {
        d->m_decreasingColor = d->m_brush.color();
        emit decreasingColorChanged();
    }

    emit d->updated();
    emit brushChanged();
}

QBrush QCandlestickSeries::brush() const
{
    Q_D(const QCandlestickSeries);

    return d->m_brush;
}

void QCandlestickSeries::setPen(const QPen &pen)
{
    Q_D(QCandlestickSeries);

    if (d->m_pen == pen)
        return;

    d->m_pen = pen;

    emit d->updated();
    emit penChanged();
}

QPen QCandlestickSeries::pen() const
{
    Q_D(const QCandlestickSeries);

    return d->m_pen;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QCandlestickSeriesPrivate::QCandlestickSeriesPrivate(QCandlestickSeries *q)
    : QAbstractSeriesPrivate(q),
      m_maximumColumnWidth(-1.0),
      m_minimumColumnWidth(5.0),
      m_bodyWidth(0.5),
      m_bodyOutlineVisible(true),
      m_capsWidth(0.5),
      m_capsVisible(false),
      m_increasingColor(QColor(Qt::transparent)),
      m_decreasingColor(QChartPrivate::defaultBrush().color()),
      m_customIncreasingColor(false),
      m_customDecreasingColor(false),
      m_brush(QChartPrivate::defaultBrush()),
      m_pen(QChartPrivate::defaultPen()),
      m_animation(nullptr)
{
}

QCandlestickSeriesPrivate::~QCandlestickSeriesPrivate()
{
    disconnect(this, 0, 0, 0);
}

void QCandlestickSeriesPrivate::initializeDomain()
{
    qreal minX(domain()->minX());
    qreal maxX(domain()->maxX());
    qreal minY(domain()->minY());
    qreal maxY(domain()->maxY());

    if (m_sets.count()) {
        QCandlestickSet *set = m_sets.first();
        minX = set->timestamp();
        maxX = set->timestamp();
        minY = set->low();
        maxY = set->high();
        for (int i = 1; i < m_sets.count(); ++i) {
            set = m_sets.at(i);
            minX = qMin(minX, qreal(set->timestamp()));
            maxX = qMax(maxX, qreal(set->timestamp()));
            minY = qMin(minY, set->low());
            maxY = qMax(maxY, set->high());
        }
        qreal extra = (maxX - minX) / m_sets.count() / 2;
        minX = minX - extra;
        maxX = maxX + extra;
    }

    domain()->setRange(minX, maxX, minY, maxY);
}

void QCandlestickSeriesPrivate::initializeAxes()
{
    foreach (QAbstractAxis* axis, m_axes) {
        if (axis->type() == QAbstractAxis::AxisTypeBarCategory) {
            if (axis->orientation() == Qt::Horizontal)
                populateBarCategories(qobject_cast<QBarCategoryAxis *>(axis));
        }
    }
}

void QCandlestickSeriesPrivate::initializeTheme(int index, ChartTheme* theme, bool forced)
{
    Q_Q(QCandlestickSeries);

    if (forced || QChartPrivate::defaultBrush() == m_brush) {
        const QList<QGradient> gradients = theme->seriesGradients();
        const QGradient gradient = gradients.at(index % gradients.size());
        const QBrush brush(ChartThemeManager::colorAt(gradient, 0.5));
        q->setBrush(brush);
    }

    if (forced || QChartPrivate::defaultPen() == m_pen) {
        QPen pen = theme->outlinePen();
        pen.setCosmetic(true);
        q->setPen(pen);
    }
}

void QCandlestickSeriesPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(QCandlestickSeries);

    CandlestickChartItem *item = new CandlestickChartItem(q, parent);
    m_item.reset(item);
    QAbstractSeriesPrivate::initializeGraphics(parent);

    if (m_chart) {
        connect(m_chart->d_ptr->m_dataset, SIGNAL(seriesAdded(QAbstractSeries *)),
                this, SLOT(handleSeriesChange(QAbstractSeries *)));
        connect(m_chart->d_ptr->m_dataset, SIGNAL(seriesRemoved(QAbstractSeries *)),
                this, SLOT(handleSeriesRemove(QAbstractSeries *)));

        item->handleCandlestickSeriesChange();
    }
}

void QCandlestickSeriesPrivate::initializeAnimations(QChart::AnimationOptions options, int duration,
                                                     QEasingCurve &curve)
{
    CandlestickChartItem *item = static_cast<CandlestickChartItem *>(m_item.data());
    Q_ASSERT(item);

    if (item->animation())
        item->animation()->stopAndDestroyLater();

    if (options.testFlag(QChart::SeriesAnimations))
        m_animation = new CandlestickAnimation(item, duration, curve);
    else
        m_animation = nullptr;
    item->setAnimation(m_animation);

    QAbstractSeriesPrivate::initializeAnimations(options, duration, curve);
}

QList<QLegendMarker *> QCandlestickSeriesPrivate::createLegendMarkers(QLegend *legend)
{
    Q_Q(QCandlestickSeries);

    QList<QLegendMarker *> list;

    return list << new QCandlestickLegendMarker(q, legend);
}

QAbstractAxis::AxisType QCandlestickSeriesPrivate::defaultAxisType(Qt::Orientation orientation) const
{
    if (orientation == Qt::Horizontal)
        return QAbstractAxis::AxisTypeBarCategory;

    if (orientation == Qt::Vertical)
        return QAbstractAxis::AxisTypeValue;

    return QAbstractAxis::AxisTypeNoAxis;
}

QAbstractAxis* QCandlestickSeriesPrivate::createDefaultAxis(Qt::Orientation orientation) const
{
    const QAbstractAxis::AxisType axisType = defaultAxisType(orientation);

    if (axisType == QAbstractAxis::AxisTypeBarCategory)
        return new QBarCategoryAxis;

    if (axisType == QAbstractAxis::AxisTypeValue)
        return new QValueAxis;

    return 0; // axisType == QAbstractAxis::AxisTypeNoAxis
}

bool QCandlestickSeriesPrivate::append(const QList<QCandlestickSet *> &sets)
{
    foreach (QCandlestickSet *set, sets) {
        if ((set == 0) || m_sets.contains(set) || set->d_ptr->m_series)
            return false; // Fail if any of the sets is null or is already appended.
        if (sets.count(set) != 1)
            return false; // Also fail if the same set occurs more than once in the given list.
    }

    foreach (QCandlestickSet *set, sets) {
        m_sets.append(set);
        connect(set->d_func(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
        connect(set->d_func(), SIGNAL(updatedCandlestick()), this, SIGNAL(updatedCandlesticks()));
        set->d_ptr->m_series = this;
    }

    return true;
}

bool QCandlestickSeriesPrivate::remove(const QList<QCandlestickSet *> &sets)
{
    if (sets.count() == 0)
        return false;

    foreach (QCandlestickSet *set, sets) {
        if ((set == 0) || (!m_sets.contains(set)))
            return false; // Fail if any of the sets is null or is not in series.
        if (sets.count(set) != 1)
            return false; // Also fail if the same set occurs more than once in the given list.
    }

    foreach (QCandlestickSet *set, sets) {
        set->d_ptr->m_series = nullptr;
        m_sets.removeOne(set);
        disconnect(set->d_func(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
        disconnect(set->d_func(), SIGNAL(updatedCandlestick()),this, SIGNAL(updatedCandlesticks()));
    }

    return true;
}

bool QCandlestickSeriesPrivate::insert(int index, QCandlestickSet *set)
{
    if ((m_sets.contains(set)) || (set == 0) || set->d_ptr->m_series)
        return false; // Fail if set is already in list or set is null.

    m_sets.insert(index, set);
    connect(set->d_func(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
    connect(set->d_func(), SIGNAL(updatedCandlestick()), this, SIGNAL(updatedCandlesticks()));
    set->d_ptr->m_series = this;

    return true;
}

void QCandlestickSeriesPrivate::handleSeriesChange(QAbstractSeries *series)
{
    Q_UNUSED(series);

    if (m_chart) {
        CandlestickChartItem *item = static_cast<CandlestickChartItem *>(m_item.data());
        if (item)
            item->handleCandlestickSeriesChange();
    }
}

void QCandlestickSeriesPrivate::handleSeriesRemove(QAbstractSeries *series)
{
    Q_Q(const QCandlestickSeries);

    QCandlestickSeries *removedSeries = static_cast<QCandlestickSeries *>(series);

    if (q == removedSeries && m_animation) {
        m_animation->stopAll();
        disconnect(m_chart->d_ptr->m_dataset, 0, removedSeries->d_func(), 0);
    }

    if (q != removedSeries) {
        CandlestickChartItem *item = static_cast<CandlestickChartItem *>(m_item.data());
        if (item)
            item->handleCandlestickSeriesChange();
    }
}

void QCandlestickSeriesPrivate::populateBarCategories(QBarCategoryAxis *axis)
{
    if (axis->categories().isEmpty()) {
        QStringList categories;
        for (int i = 0; i < m_sets.count(); ++i) {
            const qint64 timestamp = qRound64(m_sets.at(i)->timestamp());
            const QString timestampFormat = m_chart->locale().dateTimeFormat(QLocale::ShortFormat);
            categories << QDateTime::fromMSecsSinceEpoch(timestamp).toString(timestampFormat);
        }
        axis->append(categories);
    }
}

#include "moc_qcandlestickseries.cpp"
#include "moc_qcandlestickseries_p.cpp"

SCHARTS_END_NAMESPACE
