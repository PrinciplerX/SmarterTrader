#include "voidchartlayout.h"
#include <ChartPresenter>

VoidChartLayout::VoidChartLayout(ChartPresenter *presenter):
    CartesianChartLayout(presenter)
{
}

void VoidChartLayout::setGeometry(const QRectF &rect)
{
    QGraphicsLayout::setGeometry(rect);
}

