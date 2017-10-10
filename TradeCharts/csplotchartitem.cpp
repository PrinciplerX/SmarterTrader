#include "csplotchartitem.h"

CSPlotChartItem::CSPlotChartItem(CSPlotSeries *series, QGraphicsItem *item) :
    ChartItem(series->d_func(), item),
    m_series(series),
    m_animation(0)
{
    connect(series, SIGNAL(boxsetsRemoved(QList<CandleStickSet *>)), this, SLOT(handleBoxSetRemove(QList<CandleStickSet*>)));
    connect(series->d_func(), SIGNAL(restructuredBoxes()), this, SLOT(handleDataStructureChanged()));
    connect(series->d_func(), SIGNAL(updatedLayout()), this, SLOT(handleLayoutChanged()));
    connect(series->d_func(), SIGNAL(updatedBoxes()), this, SLOT(handleUpdatedBars()));
    connect(series->d_func(), SIGNAL(updated()), this, SLOT(handleUpdatedBars()));
    // QBoxPlotSeriesPrivate calls handleDataStructureChanged(), don't do it here
    setZValue(ChartPresenter::BoxPlotSeriesZValue);
}

CSPlotChartItem::~CSPlotChartItem()
{
}

void CSPlotChartItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void CSPlotChartItem::setAnimation(CSPlotAnimation *animation)
{
    m_animation = animation;
    if (m_animation) {
        foreach (CandleStick *item, m_boxTable.values())
            m_animation->addBox(item);
        handleDomainUpdated();
    }
}

void CSPlotChartItem::handleDataStructureChanged()
{
    int setCount = m_series->count();

    for (int s = 0; s < setCount; s++) {
        CandleStickSet *set = m_series->d_func()->boxSetAt(s);

        CandleStick *box = m_boxTable.value(set);
        if (!box) {
            // Item is not yet created, make a box and add it to hash table
            box = new CandleStick(set, domain(), this);
            m_boxTable.insert(set, box);
            connect(box, SIGNAL(clicked(CandleStickSet *)), m_series, SIGNAL(clicked(CandleStickSet *)));
            connect(box, SIGNAL(hovered(bool, CandleStickSet *)), m_series, SIGNAL(hovered(bool, CandleStickSet *)));
            connect(box, SIGNAL(pressed(CandleStickSet *)), m_series, SIGNAL(pressed(CandleStickSet *)));
            connect(box, SIGNAL(released(CandleStickSet *)), m_series, SIGNAL(released(CandleStickSet *)));
            connect(box, SIGNAL(doubleClicked(CandleStickSet *)),
                    m_series, SIGNAL(doubleClicked(CandleStickSet *)));
            connect(box, SIGNAL(clicked(CandleStickSet *)), set, SIGNAL(clicked()));
            connect(box, SIGNAL(hovered(bool, CandleStickSet *)), set, SIGNAL(hovered(bool)));
            connect(box, SIGNAL(pressed(CandleStickSet *)), set, SIGNAL(pressed()));
            connect(box, SIGNAL(released(CandleStickSet *)), set, SIGNAL(released()));
            connect(box, SIGNAL(doubleClicked(CandleStickSet *)), set, SIGNAL(doubleClicked()));

            // Set the decorative issues for the newly created box
            // so that the brush and pen already defined for the set are kept.
            if (set->brush() == Qt::NoBrush)
                box->setBrush(m_series->brush());
            else
                box->setBrush(set->brush());
            if (set->pen() == Qt::NoPen)
                box->setPen(m_series->pen());
            else
                box->setPen(set->pen());
            box->setBoxOutlined(m_series->boxOutlineVisible());
            box->setBoxWidth(m_series->boxWidth());
        }
        updateBoxGeometry(box, s);

        box->updateGeometry(domain());

        if (m_animation)
            m_animation->addBox(box);
    }

    handleDomainUpdated();
}

void CSPlotChartItem::handleUpdatedBars()
{
    foreach (CandleStick *item, m_boxTable.values()) {
        item->setBrush(m_series->brush());
        item->setPen(m_series->pen());
        item->setBoxOutlined(m_series->boxOutlineVisible());
        item->setBoxWidth(m_series->boxWidth());
    }
    // Override with QBoxSet specific settings
    foreach (CandleStickSet *set, m_boxTable.keys()) {
        if (set->brush().style() != Qt::NoBrush)
            m_boxTable.value(set)->setBrush(set->brush());
        if (set->pen().style() != Qt::NoPen)
            m_boxTable.value(set)->setPen(set->pen());
    }
}

void CSPlotChartItem::handleBoxSetRemove(QList<CandleStickSet *> barSets)
{
    foreach (CandleStickSet *set, barSets) {
        CandleStick *boxItem = m_boxTable.value(set);
        m_boxTable.remove(set);
        delete boxItem;
    }
}

void CSPlotChartItem::handleDomainUpdated()
{
    if ((domain()->size().width() <= 0) || (domain()->size().height() <= 0))
        return;

    // Set my bounding rect to same as domain size. Add one pixel at the top (-1.0) and the bottom as 0.0 would
    // snip a bit off from the whisker at the grid line
    m_boundingRect.setRect(0.0, -1.0, domain()->size().width(), domain()->size().height() + 1.0);

    foreach (CandleStick *item, m_boxTable.values()) {
        item->updateGeometry(domain());

        // If the animation is set, start the animation for each BoxWhisker item
        if (m_animation)
            presenter()->startAnimation(m_animation->boxAnimation(item));
    }
}

void CSPlotChartItem::handleLayoutChanged()
{
    foreach (CandleStick *item, m_boxTable.values()) {
        if (m_animation)
            m_animation->setAnimationStart(item);

        item->setBoxWidth(m_series->boxWidth());

        bool dirty = updateBoxGeometry(item, item->m_data.m_index);
        if (dirty && m_animation)
            presenter()->startAnimation(m_animation->boxChangeAnimation(item));
        else
            item->updateGeometry(domain());
    }
}

QRectF CSPlotChartItem::boundingRect() const
{
    return m_boundingRect;
}

void CSPlotChartItem::initializeLayout()
{
}

QVector<QRectF> CSPlotChartItem::calculateLayout()
{
    return QVector<QRectF>();
}

bool CSPlotChartItem::updateBoxGeometry(CandleStick *box, int index)
{
    bool changed = false;

    CandleStickSet *set = m_series->d_func()->boxSetAt(index);
    CandleStickData &data = box->m_data;

    if ((data.m_lowerExtreme != set->at(0)) || (data.m_lowerQuartile != set->at(1)) ||
        (data.m_upperQuartile != set->at(2)) || (data.m_upperExtreme != set->at(3))) {
        changed = true;
    }

    data.m_lowerExtreme = set->at(0);
    data.m_lowerQuartile = set->at(1);
    data.m_upperQuartile = set->at(2);
    data.m_upperExtreme = set->at(3);
    data.m_index = index;
    data.m_boxItems = m_series->count();

    data.m_maxX = domain()->maxX();
    data.m_minX = domain()->minX();
    data.m_maxY = domain()->maxY();
    data.m_minY = domain()->minY();

    data.m_seriesIndex = m_seriesIndex;
    data.m_seriesCount = m_seriesCount;

    return changed;
}
