#ifndef POLARCHARTLAYOUT_H
#define POLARCHARTLAYOUT_H

#include <abstractchartlayout_p.h>

SCHARTS_BEGIN_NAMESPACE

class PolarChartLayout : public AbstractChartLayout
{
public:
    PolarChartLayout(ChartPresenter *presenter);
    virtual ~PolarChartLayout();

    // from AbstractChartLayout
    QRectF calculateAxisMinimum(const QRectF &minimum, const QList<ChartAxisElement *> &axes) const;
    QRectF calculateAxisGeometry(const QRectF &geometry, const QList<ChartAxisElement *> &axes) const;
};

SCHARTS_END_NAMESPACE

#endif // POLARCHARTLAYOUT_H
