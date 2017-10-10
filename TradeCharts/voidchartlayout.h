#ifndef VOIDCHARTLAYOUT_H
#define VOIDCHARTLAYOUT_H

#include <QGraphicsLayout>
#include <CartesianChartLayout>
#include <SChartGlobal>

#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

/*! \brief 不做任何布局工作的布局管理器。用于FChart中关闭子图表的自布局功能。*/
class TRADECHARTS_EXPORT VoidChartLayout : public CartesianChartLayout
{
public:
    VoidChartLayout(ChartPresenter *presenter);
    virtual void setGeometry(const QRectF &rect) override;
};

#endif // VOIDCHARTLAYOUT_H
