#ifndef FPANELLAYOUT_H
#define FPANELLAYOUT_H

#include <QChart>
#include <AbstractChartLayout>
#include <CartesianChartLayout>

#include "tradecharts_global.h"

SCHARTS_BEGIN_NAMESPACE

class FChartPresenter;

class TRADECHARTS_EXPORT FPanelLayout : public CartesianChartLayout
{
public:
    FPanelLayout(FChartPresenter *presenter);
    virtual void setGeometry(const QRectF &rect) override;

    FChartPresenter* presenter();

    virtual void calculateItemChartGeometry(const QRectF &geometry,
                                            const QVector<ChartPresenter*>& chart,
                                            const QVector<qreal>& ratios);

private:

};

SCHARTS_END_NAMESPACE

#endif // FPANELLAYOUT_H
