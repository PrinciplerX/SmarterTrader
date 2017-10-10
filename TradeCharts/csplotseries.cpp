#include "csplotseries.h"
#include "csplotchartitem.h"
#include <QValueAxis>
#include <ChartThemeManager>
#include "candlestickset.h"
#include "qchart_p.h"
#include <ChartTheme>
#include <QBoxPlotLegendMarker>
#include <ChartDataSet>

CSPlotSeries::CSPlotSeries(QObject *parent)
    : QAbstractSeries(*new CSPlotSeriesPrivate(this), parent)
{
}

CSPlotSeries::~CSPlotSeries()
{
    Q_D(CSPlotSeries);
    if (d->m_chart)
        d->m_chart->removeSeries(this);
}

bool CSPlotSeries::append(CandleStickSet *set)
{
    Q_D(CSPlotSeries);

    bool success = d->append(set);
    if (success) {
        QList<CandleStickSet *> sets;
        sets.append(set);
        set->setParent(this);
        emit boxsetsAdded(sets);
        emit countChanged();
    }
    return success;
}

bool CSPlotSeries::remove(CandleStickSet *set)
{
    Q_D(CSPlotSeries);
    bool success = d->remove(set);
    if (success) {
        QList<CandleStickSet *> sets;
        sets.append(set);
        set->setParent(0);
        emit boxsetsRemoved(sets);
        emit countChanged();
        delete set;
        set = 0;
    }
    return success;
}

/*!
    Takes a single \a set from the series. Does not delete the boxset object.

    NOTE: The series remains as the boxset's parent object. You must set the
    parent object to take full ownership.

    Returns true if take was successful.
*/
bool CSPlotSeries::take(CandleStickSet *set)
{
    Q_D(CSPlotSeries);

    bool success = d->remove(set);
    if (success) {
        QList<CandleStickSet *> sets;
        sets.append(set);
        emit boxsetsRemoved(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Adds a list of boxsets to series. Takes ownership of the \a sets.
    Returns true, if all sets were appended successfully. If any of the sets is null or is already appended to series,
    nothing is appended and function returns false. If any of the sets is in list more than once, nothing is appended
    and function returns false.
*/
bool CSPlotSeries::append(QList<CandleStickSet *> sets)
{
    Q_D(CSPlotSeries);
    bool success = d->append(sets);
    if (success) {
        emit boxsetsAdded(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Insert a box-and-whiskers set to the series at \a index postion. Takes ownership of the \a set. If the set is null or
    is already in series, it won't be appended. Returns true, if inserting succeeded.

*/
bool CSPlotSeries::insert(int index, CandleStickSet *set)
{
    Q_D(CSPlotSeries);
    bool success = d->insert(index, set);
    if (success) {
        QList<CandleStickSet *> sets;
        sets.append(set);
        emit boxsetsAdded(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Removes all boxsets from the series. Deletes removed sets.
*/
void CSPlotSeries::clear()
{
    Q_D(CSPlotSeries);
    QList<CandleStickSet *> sets = boxSets();
    bool success = d->remove(sets);
    if (success) {
        emit boxsetsRemoved(sets);
        emit countChanged();
        foreach (CandleStickSet *set, sets)
            delete set;
    }
}

/*!
    Returns number of sets in series.
*/
int CSPlotSeries::count() const
{
    Q_D(const CSPlotSeries);
    return d->m_boxSets.count();
}

/*!
    Returns a list of sets in series. Keeps ownership of sets.
 */
QList<CandleStickSet *> CSPlotSeries::boxSets() const
{
    Q_D(const CSPlotSeries);
    return d->m_boxSets;
}

/*
    Returns QAbstractSeries::SeriesTypeCSPlot.
*/
QAbstractSeries::SeriesType CSPlotSeries::type() const
{
    return QAbstractSeries::SeriesTypeBoxPlot;
}

void CSPlotSeries::setBoxOutlineVisible(bool visible)
{
    Q_D(CSPlotSeries);

    if (d->m_boxOutlineVisible != visible) {
        d->m_boxOutlineVisible = visible;
        emit d->updated();
        emit boxOutlineVisibilityChanged();
    }
}

bool CSPlotSeries::boxOutlineVisible()
{
    Q_D(CSPlotSeries);

    return d->m_boxOutlineVisible;
}

void CSPlotSeries::setBoxWidth(qreal width)
{
    Q_D(CSPlotSeries);

    if (width != d->m_boxWidth) {
        if (width < 0.0)
            width = 0.0;
        if (width > 1.0)
            width = 1.0;
        d->m_boxWidth = width;
        emit d->updatedLayout();
        emit boxWidthChanged();
    }
}

qreal CSPlotSeries::boxWidth()
{
    Q_D(CSPlotSeries);

    return d->m_boxWidth;
}

void CSPlotSeries::setBrush(const QBrush &brush)
{
    Q_D(CSPlotSeries);

    if (d->m_brush != brush) {
        d->m_brush = brush;
        emit d->updated();
        emit brushChanged();
    }
}

QBrush CSPlotSeries::brush() const
{
    Q_D(const CSPlotSeries);

    return d->m_brush;
}

void CSPlotSeries::setPen(const QPen &pen)
{
    Q_D(CSPlotSeries);

    if (d->m_pen != pen) {
        d->m_pen = pen;
        emit d->updated();
        emit penChanged();
    }
}

QPen CSPlotSeries::pen() const
{
    Q_D(const CSPlotSeries);

    return d->m_pen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSPlotSeriesPrivate::CSPlotSeriesPrivate(CSPlotSeries *q)
    : QAbstractSeriesPrivate(q),
      m_pen(QChartPrivate::defaultPen()),
      m_brush(QChartPrivate::defaultBrush()),
      m_boxOutlineVisible(true),
      m_boxWidth(0.5)
{
}

CSPlotSeriesPrivate::~CSPlotSeriesPrivate()
{
    disconnect(this, 0, 0, 0);
}

void CSPlotSeriesPrivate::initializeDomain()
{
    qreal minX(domain()->minX());
    qreal minY(domain()->minY());
    qreal maxX(domain()->maxX());
    qreal maxY(domain()->maxY());

    qreal x = m_boxSets.count();
    minX = qMin(minX, qreal(-0.5));
    minY = min();
    maxX = qMax(maxX, x - qreal(0.5));
    maxY = qMax(maxY, max());

    domain()->setRange(minX, maxX, minY, maxY);
}

void CSPlotSeriesPrivate::initializeAxes()
{
    foreach (QAbstractAxis* axis, m_axes) {
        if (axis->type() == QAbstractAxis::AxisTypeBarCategory) {
            if (axis->orientation() == Qt::Horizontal)
                populateCategories(qobject_cast<QBarCategoryAxis *>(axis));
        }
    }
}

QAbstractAxis::AxisType CSPlotSeriesPrivate::defaultAxisType(Qt::Orientation orientation) const
{
    if (orientation == Qt::Horizontal)
        return QAbstractAxis::AxisTypeBarCategory;

    return QAbstractAxis::AxisTypeValue;
}

QAbstractAxis* CSPlotSeriesPrivate::createDefaultAxis(Qt::Orientation orientation) const
{
    if (defaultAxisType(orientation) == QAbstractAxis::AxisTypeBarCategory)
        return new QBarCategoryAxis;
    else
        return new QValueAxis;
}

void CSPlotSeriesPrivate::populateCategories(QBarCategoryAxis *axis)
{
    QStringList categories;
    if (axis->categories().isEmpty()) {
        for (int i(1); i < m_boxSets.count() + 1; i++) {
            CandleStickSet *set = m_boxSets.at(i - 1);
            if (set->label().isEmpty())
                categories << presenter()->numberToString(i);
            else
                categories << set->label();
        }
        axis->append(categories);
    }
}

void CSPlotSeriesPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(CSPlotSeries);

    CSPlotChartItem *CSPlot = new CSPlotChartItem(q, parent);
    m_item.reset(CSPlot);
    QAbstractSeriesPrivate::initializeGraphics(parent);

    if (m_chart) {
        connect(m_chart->dPtr()->m_dataset, SIGNAL(seriesAdded(QAbstractSeries*)), this, SLOT(handleSeriesChange(QAbstractSeries*)) );
        connect(m_chart->dPtr()->m_dataset, SIGNAL(seriesRemoved(QAbstractSeries*)), this, SLOT(handleSeriesRemove(QAbstractSeries*)) );

        QList<QAbstractSeries *> serieses = m_chart->series();

        // Tries to find this series from the Chart's list of series and deduce the index
        int index = 0;
        foreach (QAbstractSeries *s, serieses) {
            if (s->type() == QAbstractSeries::SeriesTypeBoxPlot) {
                if (q == static_cast<CSPlotSeries *>(s)) {
                    CSPlot->m_seriesIndex = index;
                    m_index = index;
                }
                index++;
            }
        }
        CSPlot->m_seriesCount = index;
    }

    // Make CSPlotChartItem to instantiate box & whisker items
    CSPlot->handleDataStructureChanged();
}

void CSPlotSeriesPrivate::initializeTheme(int index, ChartTheme* theme, bool forced)
{
    Q_Q(CSPlotSeries);

    const QList<QGradient> gradients = theme->seriesGradients();

    if (forced || QChartPrivate::defaultBrush() == m_brush) {
        QColor brushColor = ChartThemeManager::colorAt(gradients.at(index % gradients.size()), 0.5);
        q->setBrush(brushColor);
    }

    if (forced || QChartPrivate::defaultPen() == m_pen) {
        QPen pen = theme->outlinePen();
        pen.setCosmetic(true);
        q->setPen(pen);
    }
}

void CSPlotSeriesPrivate::initializeAnimations(QChart::AnimationOptions options, int duration,
                                                 QEasingCurve &curve)
{
    CSPlotChartItem *item = static_cast<CSPlotChartItem *>(m_item.data());
    Q_ASSERT(item);
    if (item->animation())
        item->animation()->stopAndDestroyLater();

    if (options.testFlag(QChart::SeriesAnimations))
        m_animation = new CSPlotAnimation(item, duration, curve);
    else
        m_animation = 0;
    item->setAnimation(m_animation);

    QAbstractSeriesPrivate::initializeAnimations(options, duration, curve);

    // Make CSPlotChartItem to instantiate box & whisker items
    item->handleDataStructureChanged();
}

QList<QLegendMarker*> CSPlotSeriesPrivate::createLegendMarkers(QLegend *legend)
{
    Q_Q(CSPlotSeries);
    QList<QLegendMarker *> list;
    //return list << new QBoxPlotLegendMarker(q, legend);
    return QList<QLegendMarker*>();
}

void CSPlotSeriesPrivate::handleSeriesRemove(QAbstractSeries *series)
{
    Q_Q(CSPlotSeries);

    CSPlotSeries *removedSeries = static_cast<CSPlotSeries *>(series);

    if (q == removedSeries && m_animation) {
        m_animation->stopAll();
        QObject::disconnect(m_chart->dPtr()->m_dataset, 0, removedSeries->d_func(), 0);
    }

    // Test if series removed is me, then don't do anything
    if (q != removedSeries) {
        CSPlotChartItem *item = static_cast<CSPlotChartItem *>(m_item.data());
        if (item) {
            item->m_seriesCount = item->m_seriesCount - 1;
            if (removedSeries->d_func()->m_index < m_index) {
                m_index--;
                item->m_seriesIndex = m_index;
            }

            item->handleDataStructureChanged();
        }
    }
}

void CSPlotSeriesPrivate::handleSeriesChange(QAbstractSeries *series)
{
    Q_UNUSED(series);

    Q_Q(CSPlotSeries);

    CSPlotChartItem *CSPlot = static_cast<CSPlotChartItem *>(m_item.data());

    if (m_chart) {
        QList<QAbstractSeries *> serieses = m_chart->series();

        // Tries to find this series from the Chart's list of series and deduce the index
        int index = 0;
        foreach (QAbstractSeries *s, serieses) {
            if (s->type() == QAbstractSeries::SeriesTypeBoxPlot) {
                if (q == static_cast<CSPlotSeries *>(s)) {
                    CSPlot->m_seriesIndex = index;
                    m_index = index;
                }
                index++;
            }
        }
        CSPlot->m_seriesCount = index;
    }

    CSPlot->handleDataStructureChanged();
}

bool CSPlotSeriesPrivate::append(CandleStickSet *set)
{
    if (m_boxSets.contains(set) || (set == 0) || set->dPtr()->m_series)
        return false; // Fail if set is already in list or set is null.

    m_boxSets.append(set);
    QObject::connect(set->dPtr().data(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
    QObject::connect(set->dPtr().data(), SIGNAL(updatedBox()), this, SIGNAL(updatedBoxes()));
    QObject::connect(set->dPtr().data(), SIGNAL(restructuredBox()), this, SIGNAL(restructuredBoxes()));
    set->dPtr()->setSeries(this);

    emit restructuredBoxes(); // this notifies CSPlotchartitem
    return true;
}

bool CSPlotSeriesPrivate::remove(CandleStickSet *set)
{
    if (!m_boxSets.contains(set))
        return false; // Fail if set is not in list

    set->dPtr()->m_series = 0;
    m_boxSets.removeOne(set);
    QObject::disconnect(set->dPtr().data(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
    QObject::disconnect(set->dPtr().data(), SIGNAL(updatedBox()), this, SIGNAL(updatedBoxes()));
    QObject::disconnect(set->dPtr().data(), SIGNAL(restructuredBox()), this, SIGNAL(restructuredBoxes()));

    emit restructuredBoxes(); // this notifies CSPlotchartitem
    return true;
}

bool CSPlotSeriesPrivate::append(QList<CandleStickSet *> sets)
{
    foreach (CandleStickSet *set, sets) {
        if ((set == 0) || m_boxSets.contains(set) || set->dPtr()->m_series)
            return false; // Fail if any of the sets is null or is already appended.
        if (sets.count(set) != 1)
            return false; // Also fail if same set is more than once in given list.
    }

    foreach (CandleStickSet *set, sets) {
        m_boxSets.append(set);
        QObject::connect(set->dPtr().data(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
        QObject::connect(set->dPtr().data(), SIGNAL(updatedBox()), this, SIGNAL(updatedBoxes()));
        QObject::connect(set->dPtr().data(), SIGNAL(restructuredBox()), this, SIGNAL(restructuredBoxes()));
        set->dPtr()->setSeries(this);
    }

    emit restructuredBoxes(); // this notifies CSPlotchartitem
    return true;
}

bool CSPlotSeriesPrivate::remove(QList<CandleStickSet *> sets)
{
    if (sets.count() == 0)
        return false;

    foreach (CandleStickSet *set, sets) {
        if ((set == 0) || (!m_boxSets.contains(set)))
            return false; // Fail if any of the sets is null or is not in series
        if (sets.count(set) != 1)
            return false; // Also fail if same set is more than once in given list.
    }

    foreach (CandleStickSet *set, sets) {
        set->dPtr()->m_series = 0;
        m_boxSets.removeOne(set);
        QObject::disconnect(set->dPtr().data(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
        QObject::disconnect(set->dPtr().data(), SIGNAL(updatedBox()), this, SIGNAL(updatedBoxes()));
        QObject::disconnect(set->dPtr().data(), SIGNAL(restructuredBox()), this, SIGNAL(restructuredBoxes()));
    }

    emit restructuredBoxes();        // this notifies CSPlotchartitem

    return true;
}

bool CSPlotSeriesPrivate::insert(int index, CandleStickSet *set)
{
    if ((m_boxSets.contains(set)) || (set == 0) || set->dPtr()->m_series)
        return false; // Fail if set is already in list or set is null.

    m_boxSets.insert(index, set);
    set->dPtr()->setSeries(this);
    QObject::connect(set->dPtr().data(), SIGNAL(updatedLayout()), this, SIGNAL(updatedLayout()));
    QObject::connect(set->dPtr().data(), SIGNAL(updatedBox()), this, SIGNAL(updatedBoxes()));
    QObject::connect(set->dPtr().data(), SIGNAL(restructuredBox()), this, SIGNAL(restructuredBoxes()));

    emit restructuredBoxes();      // this notifies CSPlotchartitem
    return true;
}

CandleStickSet *CSPlotSeriesPrivate::boxSetAt(int index)
{
    return m_boxSets.at(index);
}

qreal CSPlotSeriesPrivate::min()
{
    if (m_boxSets.count() <= 0)
        return 0;

    qreal min = m_boxSets.at(0)->at(0);

    foreach (CandleStickSet *set, m_boxSets) {
        for (int i = 0; i < 4; i++) {
            if (set->at(i) < min)
                min = set->at(i);
        }
    }

    return min;
}

qreal CSPlotSeriesPrivate::max()
{
    if (m_boxSets.count() <= 0)
        return 0;

    qreal max = m_boxSets.at(0)->at(0);

    foreach (CandleStickSet *set, m_boxSets) {
        for (int i = 0; i < 4; i++) {
            if (set->at(i) > max)
                max = set->at(i);
        }
    }

    return max;
}
