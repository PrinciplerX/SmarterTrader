#ifndef QSPLINESERIES_H
#define QSPLINESERIES_H

#include <SChartGlobal>
#include <QLineSeries>
#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtCore/QtGlobal>

SCHARTS_BEGIN_NAMESPACE

class QSplineSeriesPrivate;

class SCHARTS_EXPORT QSplineSeries : public QLineSeries
{
    Q_OBJECT

public:
    explicit QSplineSeries(QObject *parent = Q_NULLPTR);
    ~QSplineSeries();
    QAbstractSeries::SeriesType type() const;

private:
    Q_DECLARE_PRIVATE(QSplineSeries)
    Q_DISABLE_COPY(QSplineSeries)
    friend class SplineChartItem;
};

SCHARTS_END_NAMESPACE

#endif // QSPLINESERIES_H
