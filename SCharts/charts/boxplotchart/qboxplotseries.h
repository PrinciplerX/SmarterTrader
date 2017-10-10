#ifndef QBOXPLOTSERIES_H
#define QBOXPLOTSERIES_H

#include <SChartGlobal>
#include <QBoxSet>
#include "../qabstractseries.h"

SCHARTS_BEGIN_NAMESPACE

class QBoxPlotSeriesPrivate;

class SCHARTS_EXPORT QBoxPlotSeries : public QAbstractSeries
{
    Q_OBJECT
    Q_PROPERTY(bool boxOutlineVisible READ boxOutlineVisible WRITE setBoxOutlineVisible NOTIFY boxOutlineVisibilityChanged)
    Q_PROPERTY(qreal boxWidth READ boxWidth WRITE setBoxWidth NOTIFY boxWidthChanged)
    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged REVISION 1)
public:
    explicit QBoxPlotSeries(QObject *parent = Q_NULLPTR);
    ~QBoxPlotSeries();

    bool append(QBoxSet *box);
    bool remove(QBoxSet *box);
    bool take(QBoxSet *box);
    bool append(QList<QBoxSet *> boxes);
    bool insert(int index, QBoxSet *box);
    int count() const;
    QList<QBoxSet *> boxSets() const;
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
    void clicked(QBoxSet *boxset);
    void hovered(bool status, QBoxSet *boxset);
    void pressed(QBoxSet *boxset);
    void released(QBoxSet *boxset);
    void doubleClicked(QBoxSet *boxset);
    void countChanged();
    void penChanged();
    void brushChanged();
    void boxOutlineVisibilityChanged();
    void boxWidthChanged();

    void boxsetsAdded(QList<QBoxSet *> sets);
    void boxsetsRemoved(QList<QBoxSet *> sets);

private:
    Q_DECLARE_PRIVATE(QBoxPlotSeries)
    Q_DISABLE_COPY(QBoxPlotSeries)
    friend class BoxPlotChartItem;
    friend class QBoxPlotLegendMarkerPrivate;
};

SCHARTS_END_NAMESPACE

#endif // QBOXPLOTSERIES_H
