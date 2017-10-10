#ifndef MACD_H
#define MACD_H

#include "plotableitem.h"
#include <tradedata.h>
#include "techniqueindicator.h"
#include "operation.h"
#include "plotablecurve.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT MACD : public PlotableItem, public TechniqueIndicator
{
public:
    MACD(AbstractHistoryData* data,
         int macdShortPeriod = 12,
         int macdLongPeriod = 26,
         int macdDifferiod = 9);

    virtual void draw(const QVector<int>& itemPosX,
                      TradingCurveWidgets::IndexRange indexRange,
                      int bodyWidth,
                      const QwtScaleMap& yMap,
                      QPainter* painter) override;

    /*! \sa PlotableSequence::regionalMaxValue() */
    virtual double regionalMaxValue(int minIndex,int maxIndex) const override;

    /*! \sa PlotableSequence::regionalMinValue() */
    virtual double regionalMinValue(int minIndex, int maxIndex) const override;

    virtual void onDataLoaded() override;
    virtual void onDataUpdated() override;
    virtual void onDataAppended() override;

    virtual int size() const override { return fMacd.size(); }

    /*! \sa PlotableSequence::dateString() */
    virtual QString dateString(int index) override;
    virtual QString titleLabelString(int index) override;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const override {
        return yMap.transform(fMacd.at(itemIndex));
    }

    virtual void createTitleCache();

    virtual QString category() const override { return mCategory; }

private:
    QVector<QString> titleCache;

    /*! 均线周期 */
    int shortPeriod;
    int longPeriod;
    int diffPeriod;

    ReusableOperationWrapper<double> fDiff;
    ReusableOperationWrapper<double> fDea;
    ReusableOperationWrapper<double> fMacd;

    PlotableCurve* diff;
    PlotableCurve* dea;

    AbstractHistoryData* mHistoryData;

    static const QString mCategory;
};

#endif // MACD_H
