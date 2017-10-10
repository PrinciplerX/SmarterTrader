#ifndef FCHARTLAYOUT_H
#define FCHARTLAYOUT_H

#include <QGraphicsLayout>
#include <CartesianChartLayout>
#include <SChartGlobal>

#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

class TRADECHARTS_EXPORT FChartLayout : public CartesianChartLayout
{
public:
    FChartLayout(ChartPresenter *presenter);
    virtual void setGeometry(const QRectF &rect) override;

protected:
    virtual QRectF calculateLegendAndAxisGeometry(const QRectF &geometry, QLegend *legend,
                                                  const QList<ChartAxisElement *> &axes) const;
};

#endif // FCHARTLAYOUT_H
