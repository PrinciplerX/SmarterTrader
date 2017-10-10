#ifndef VOIDCHARTLAYOUT_H
#define VOIDCHARTLAYOUT_H

#include <QGraphicsLayout>
#include <CartesianChartLayout>
#include <SChartGlobal>

#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

/*! \brief �����κβ��ֹ����Ĳ��ֹ�����������FChart�йر���ͼ����Բ��ֹ��ܡ�*/
class TRADECHARTS_EXPORT VoidChartLayout : public CartesianChartLayout
{
public:
    VoidChartLayout(ChartPresenter *presenter);
    virtual void setGeometry(const QRectF &rect) override;
};

#endif // VOIDCHARTLAYOUT_H
