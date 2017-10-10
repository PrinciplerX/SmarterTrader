#ifndef QLINESERIES_H
#define QLINESERIES_H

#include <SChartGlobal>
#include <QXYSeries>
#include <QtGui/QPen>

SCHARTS_BEGIN_NAMESPACE

class QLineSeriesPrivate;

class SCHARTS_EXPORT QLineSeries : public QXYSeries
{
    Q_OBJECT

public:
    explicit QLineSeries(QObject *parent = Q_NULLPTR);
    ~QLineSeries();
    QAbstractSeries::SeriesType type() const;

protected:
    QLineSeries(QLineSeriesPrivate &d, QObject *parent = Q_NULLPTR);

private:
    Q_DECLARE_PRIVATE(QLineSeries)
    Q_DISABLE_COPY(QLineSeries)
    friend class LineChartItem;
};

SCHARTS_END_NAMESPACE

#endif // QLINESERIES_H
