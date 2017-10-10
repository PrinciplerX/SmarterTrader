#ifndef BOXPLOTCHARTITEM_H
#define BOXPLOTCHARTITEM_H

#include <boxwhiskers_p.h>
#include "qboxplotseries.h"
#include <chartitem_p.h>
#include <boxplotanimation_p.h>
#include <QBoxSet>
#include <QtWidgets/QGraphicsItem>

SCHARTS_BEGIN_NAMESPACE

class BoxPlotChartItem : public ChartItem
{
    Q_OBJECT
public:
    BoxPlotChartItem(QBoxPlotSeries *series, QGraphicsItem *item = 0);
    ~BoxPlotChartItem();

    void setAnimation(BoxPlotAnimation *animation);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

public Q_SLOTS:
    void handleDataStructureChanged();
    void handleDomainUpdated();
    void handleLayoutChanged();
    void handleUpdatedBars();
    void handleBoxsetRemove(QList<QBoxSet *> barSets);

private:
    virtual QVector<QRectF> calculateLayout();
    void initializeLayout();
    bool updateBoxGeometry(BoxWhiskers *box, int index);

protected:
    friend class QBoxPlotSeriesPrivate;
    QBoxPlotSeries *m_series; // Not owned.
    QList<BoxWhiskers *> m_boxes;
    QHash<QBoxSet *, BoxWhiskers *> m_boxTable;
    int m_seriesIndex;
    int m_seriesCount;

    BoxPlotAnimation *m_animation;

    QRectF m_boundingRect;
};

SCHARTS_END_NAMESPACE

#endif // BOXPLOTCHARTITEM_H
