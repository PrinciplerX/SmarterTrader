#ifndef KDJ_H
#define KDJ_H

#include "tradingcurvewidgets_global.h"
#include "plotableitem.h"
#include "plotablecurve.h"
#include "operation.h"
#include "techniqueindicator.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT KDJ : public PlotableItem, public TechniqueIndicator
{
public:
    KDJ(AbstractHistoryData* data, int rsvPeriod = 9, int kPeriod = 3, int dPeriod = 3);

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
        return yMap.transform(k[itemIndex]);
    }

    virtual void createTitleCache();

    virtual QString category() const override { return mCategory; }

private:
    QVector<QString> titleCache;

    int rsvPeriod;
    int kPeriod;
    int dPeriod;

    PlotableCurve k;
    PlotableCurve d;
    PlotableCurve j;

    static const QString mCategory;
};

#endif // KDJ_H
