#ifndef ASI_H
#define ASI_H

#include "tradingcurvewidgets_global.h"
#include "plotablecurve.h"
#include "operation.h"
#include "techniqueindicator.h"

class ASI : public PlotableCurve, public TechniqueIndicator
{
public:
    ASI(AbstractHistoryData* data, int period);

    virtual void onDataLoaded() override;
    virtual void onDataUpdated() override;
    virtual void onDataAppended() override;

    virtual QString category() const override { return mCategory; }

private:
    int mPeriod;

    PlotableCurve cAsi;

    static const QString mCategory;
};

#endif // ASI_H
