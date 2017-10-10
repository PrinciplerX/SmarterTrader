#ifndef BOLL_H
#define BOLL_H

#include "tradingcurvewidgets_global.h"
#include "plotableitem.h"
#include "bar.h"
#include "candlestick.h"
#include "plotablecurve.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT BOLL : public PlotableItem, public TechniqueIndicator
{
public:
    BOLL(AbstractHistoryData* data, int period = 20, double times = 2.0);

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

    virtual int size() const override;

    /*! \sa PlotableSequence::dateString() */
    virtual QString dateString(int index) override;
    virtual QString titleLabelString(int index) override;

    virtual int tracerTarget(int itemIndex, const QwtScaleMap& yMap) const override {
        return yMap.transform(mid->at(itemIndex));
    }

    virtual void createTitleCache();

    virtual QString category() const override { return mCategory; }

private:
    QVector<QString> titleCache;

    //上包络和下包络与均值差值是标准差的倍数
    double mTimes;

    ReusableOperationWrapper<double> std;
    CandleStick bar;

    PlotableCurve* upper;
    PlotableCurve* lower;
    PlotableCurve* mid;

    static const QString mCategory;
};

#endif // BOLL_H
