#ifndef CSPLOTSERIES_H
#define CSPLOTSERIES_H

#include "qabstractseries_p.h"
#include "candlestick.h"
#include <qabstractseries.h>
#include <QBarCategoryAxis>
#include "csplotanimation.h"

#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

class CSPlotSeriesPrivate;

class TRADECHARTS_EXPORT CSPlotSeries : public QAbstractSeries
{
    Q_OBJECT
    Q_PROPERTY(bool boxOutlineVisible READ boxOutlineVisible WRITE setBoxOutlineVisible NOTIFY boxOutlineVisibilityChanged)
    Q_PROPERTY(qreal boxWidth READ boxWidth WRITE setBoxWidth NOTIFY boxWidthChanged)
    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged REVISION 1)
public:
    explicit CSPlotSeries(QObject *parent = Q_NULLPTR);
    ~CSPlotSeries();

    bool append(CandleStickSet *box);
    bool remove(CandleStickSet *box);
    bool take(CandleStickSet *box);
    bool append(QList<CandleStickSet *> boxes);
    bool insert(int index, CandleStickSet *box);
    int count() const;
    QList<CandleStickSet *> boxSets() const;
    void clear();

    QAbstractSeries::SeriesType type() const;

    void setBoxOutlineVisible(bool visible);
    bool boxOutlineVisible();
    void setBoxWidth(qreal width);
    qreal boxWidth();
    void setBrush(const QBrush &brush);
    QBrush brush() const;
    void setPen(const QPen &pen);
    QPen pen() const;

Q_SIGNALS:
    void clicked(CandleStickSet *boxset);
    void hovered(bool status, CandleStickSet *boxset);
    void pressed(CandleStickSet *boxset);
    void released(CandleStickSet *boxset);
    void doubleClicked(CandleStickSet *boxset);
    void countChanged();
    void penChanged();
    void brushChanged();
    void boxOutlineVisibilityChanged();
    void boxWidthChanged();

    void boxsetsAdded(QList<CandleStickSet *> sets);
    void boxsetsRemoved(QList<CandleStickSet *> sets);

private:
    Q_DECLARE_PRIVATE(CSPlotSeries)
    Q_DISABLE_COPY(CSPlotSeries)
    friend class CSPlotChartItem;
    friend class CSPlotLegendMarkerPrivate;
};

class CSPlotSeriesPrivate : public QAbstractSeriesPrivate
{
    Q_OBJECT

public:
    CSPlotSeriesPrivate(CSPlotSeries *q);
    ~CSPlotSeriesPrivate();

    void initializeGraphics(QGraphicsItem *parent);
    void initializeDomain();
    void initializeAxes();
    void initializeAnimations(QChart::AnimationOptions options, int duration, QEasingCurve &curve);
    void initializeTheme(int index, ChartTheme *theme, bool forced = false);

    QList<QLegendMarker*> createLegendMarkers(QLegend *legend);

    virtual QAbstractAxis::AxisType defaultAxisType(Qt::Orientation orientation) const;
    QAbstractAxis *createDefaultAxis(Qt::Orientation orientation) const;

    bool append(CandleStickSet *set);
    bool remove(CandleStickSet *set);
    bool append(QList<CandleStickSet *> sets);
    bool remove(QList<CandleStickSet *> sets);
    bool insert(int index, CandleStickSet *set);
    CandleStickSet *boxSetAt(int index);

    qreal max();
    qreal min();

private:
    void populateCategories(QBarCategoryAxis *axis);

Q_SIGNALS:
    void updated();
    void clicked(int index, CandleStickSet *barset);
    void pressed(int index, CandleStickSet *barset);
    void released(int index, CandleStickSet *barset);
    void doubleClicked(int index, CandleStickSet *barset);
    void updatedBoxes();
    void updatedLayout();
    void restructuredBoxes();

private Q_SLOTS:
    void handleSeriesChange(QAbstractSeries *series);
    void handleSeriesRemove(QAbstractSeries *series);

protected:
    QList<CandleStickSet *> m_boxSets;
    QPen m_pen;
    QBrush m_brush;
    bool m_boxOutlineVisible;
    int m_index;
    CSPlotAnimation *m_animation;
    qreal m_boxWidth;

private:
    Q_DECLARE_PUBLIC(CSPlotSeries)
};

#endif // CSPLOTSERIES_H
