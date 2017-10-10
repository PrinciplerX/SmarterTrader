#ifndef GLOBALAVERAGE_H
#define GLOBALAVERAGE_H

#include "plotableitem.h"
#include <techniqueindicator.h>
#include <tradedata.h>
#include "plotablecurve.h"
#include "operation.h"

/*!
 * \brief 收盘价格线，根据初始化数据的周期可用于表示实时价格曲线显示和历史数据。
 */
class TRADINGCURVEWIDGETSSHARED_EXPORT GlobalAverage : public PlotableItem, public TechniqueIndicator
{
public:
    GlobalAverage(AbstractHistoryData* data,
                  const QPen& pen = QPen(Qt::yellow));

    virtual void draw(const QVector<int>&                        itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int                                 bodyWidth,
                      const QwtScaleMap&                  yMap,
                      QPainter*                           painter) override;

    virtual double regionalMaxValue(int minIndex,int maxIndex) const override;
    virtual double regionalMinValue(int minIndex, int maxIndex) const override;

    virtual void onDataLoaded() override;
    virtual void onDataUpdated() override;
    virtual void onDataAppended() override;

    virtual int size() const override;

    virtual QString dateString(int index) override;
    virtual QString titleLabelString(int index) override;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const override;

    virtual QString category() const override { return mCategory; }

private:
    PlotableCurve curve;
    static const QString mCategory;
    AbstractHistoryData* mHistoryData;
};
#endif // GLOBALAVERAGE_H
