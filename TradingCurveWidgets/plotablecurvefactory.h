#ifndef PLOTABLECURVEFACTORY_H
#define PLOTABLECURVEFACTORY_H

#include "tradingcurvewidgets_global.h"
#include "plotablecurve.h"
#include "operation.h"
#include "tradedataoperations.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT PlotableCurveFactory
{
public:
    static PlotableCurve* createCLOSE(AbstractHistoryData* data);
    static PlotableCurve* createEMA(AbstractHistoryData* data, int period);
    static PlotableCurve* createMA(AbstractHistoryData* data, int period);
    static PlotableCurve* createMAVOL(AbstractHistoryData* data, int period);
    static PlotableCurve* createBIAS(AbstractHistoryData* data, int period);
    static PlotableCurve* createWMS(AbstractHistoryData* data, int period);
    static PlotableCurve* createLLV(AbstractHistoryData* data, int period);
    static PlotableCurve* createHHV(AbstractHistoryData* data, int period);
    static PlotableCurve* createTR(AbstractHistoryData* data);
    static PlotableCurve* createATR(AbstractHistoryData* data, int period);
    static PlotableCurve* createOBV(AbstractHistoryData* data);
};

#endif // PLOTABLECURVEFACTORY_H
