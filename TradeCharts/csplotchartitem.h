#ifndef CSPLOTCHARTITEM_H
#define CSPLOTCHARTITEM_H

#include "candlestick.h"
#include "csplotanimation.h"
#include <csplotseries.h>
#include <chartitem_p.h>
#include "candlestickset.h"
#include <QGraphicsItem>

#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

class TRADECHARTS_EXPORT CSPlotChartItem : public ChartItem
{
    Q_OBJECT
public:
    CSPlotChartItem(CSPlotSeries* series, QGraphicsItem* item = 0);
    ~CSPlotChartItem();

    void setAnimation(CSPlotAnimation* animation);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

public slots:
    void handleDataStructureChanged();
    void handleDomainUpdated();
    void handleLayoutChanged();
    void handleUpdatedBars();
    void handleBoxSetRemove(QList<CandleStickSet*> barSets);

private:
    virtual QVector<QRectF> calculateLayout();
    void initializeLayout();
    bool updateBoxGeometry(CandleStick* box, int index);

protected:
    friend class CSPlotSeriesPrivate;
    CSPlotSeries *m_series; // Not owned.
    QList<CandleStick *> m_boxes;
    QHash<CandleStickSet *, CandleStick *> m_boxTable;
    int m_seriesIndex;
    int m_seriesCount;

    CSPlotAnimation *m_animation;

    QRectF m_boundingRect;
};

#endif // CSPLOTCHARTITEM_H
