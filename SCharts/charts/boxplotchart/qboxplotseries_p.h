#ifndef QBOXPLOTSERIES_P_H
#define QBOXPLOTSERIES_P_H

#include "qboxplotseries.h"
#include <qabstractbarseries_p.h>
#include <abstractdomain_p.h>
#include <QBarSet>

SCHARTS_BEGIN_NAMESPACE

class BoxPlotAnimation;

class QBoxPlotSeriesPrivate : public QAbstractSeriesPrivate
{
    Q_OBJECT

public:
    QBoxPlotSeriesPrivate(QBoxPlotSeries *q);
    ~QBoxPlotSeriesPrivate();

    void initializeGraphics(QGraphicsItem *parent);
    void initializeDomain();
    void initializeAxes();
    void initializeAnimations(QChart::AnimationOptions options, int duration, QEasingCurve &curve);
    void initializeTheme(int index, ChartTheme *theme, bool forced = false);

    QList<QLegendMarker*> createLegendMarkers(QLegend *legend);

    virtual QAbstractAxis::AxisType defaultAxisType(Qt::Orientation orientation) const;
    QAbstractAxis *createDefaultAxis(Qt::Orientation orientation) const;

    bool append(QBoxSet *set);
    bool remove(QBoxSet *set);
    bool append(QList<QBoxSet *> sets);
    bool remove(QList<QBoxSet *> sets);
    bool insert(int index, QBoxSet *set);
    QBoxSet *boxSetAt(int index);

    qreal max();
    qreal min();

private:
    void populateCategories(QBarCategoryAxis *axis);

Q_SIGNALS:
    void updated();
    void clicked(int index, QBoxSet *barset);
    void pressed(int index, QBoxSet *barset);
    void released(int index, QBoxSet *barset);
    void doubleClicked(int index, QBoxSet *barset);
    void updatedBoxes();
    void updatedLayout();
    void restructuredBoxes();

private Q_SLOTS:
    void handleSeriesChange(QAbstractSeries *series);
    void handleSeriesRemove(QAbstractSeries *series);

protected:
    QList<QBoxSet *> m_boxSets;
    QPen m_pen;
    QBrush m_brush;
    bool m_boxOutlineVisible;
    int m_index;
    BoxPlotAnimation *m_animation;
    qreal m_boxWidth;

private:
    Q_DECLARE_PUBLIC(QBoxPlotSeries)
};

SCHARTS_END_NAMESPACE

#endif
