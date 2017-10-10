#ifndef DMI_H
#define DMI_H

#include "tradingcurvewidgets_global.h"
#include "plotableitem.h"
#include <techniqueindicator.h>
#include "plotablecurve.h"
#include "operation.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT DMI : public PlotableItem, public TechniqueIndicator
{
public:
    DMI(AbstractHistoryData* data, int sumPeriod = 14, int adxPeriod = 6);

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
        return yMap.transform(adx[itemIndex]);
    }

    virtual void createTitleCache();

    virtual QString category() const override { return mCategory; }

private:
    QVector<QString> titleCache;

    PlotableCurve pDI;
    PlotableCurve nDI;
    PlotableCurve adx;
    PlotableCurve adxr;

    int sumPeriod;
    int adxPeriod;

    static const QString mCategory;

    AbstractHistoryData* mHistoryData;
};

#endif // DMI_H
