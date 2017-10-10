#ifndef CARTESIANCHARTLAYOUT_H
#define CARTESIANCHARTLAYOUT_H

#include <AbstractChartLayout>

SCHARTS_BEGIN_NAMESPACE

class SCHARTS_EXPORT CartesianChartLayout : public AbstractChartLayout
{
public:
    CartesianChartLayout(ChartPresenter *presenter);
    virtual ~CartesianChartLayout();

    // from AbstractChartLayout
    QRectF calculateAxisMinimum(const QRectF &minimum, const QList<ChartAxisElement *> &axes) const;
    QRectF calculateAxisGeometry(const QRectF &geometry, const QList<ChartAxisElement *> &axes) const;
};

SCHARTS_END_NAMESPACE

#endif // CARTESIANCHARTLAYOUT_H
