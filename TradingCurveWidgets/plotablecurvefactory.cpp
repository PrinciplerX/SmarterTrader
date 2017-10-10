#include "plotablecurvefactory.h"

PlotableCurve *PlotableCurveFactory::createCLOSE(AbstractHistoryData *data)
{
    return new PlotableCurve(new CLOSE(data));
}

PlotableCurve *PlotableCurveFactory::createEMA(AbstractHistoryData *data, int period)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new EMA(new CLOSE(data), period)));
}

PlotableCurve *PlotableCurveFactory::createMA(AbstractHistoryData *data, int period)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new MA(new CLOSE(data), period)));
}

PlotableCurve *PlotableCurveFactory::createMAVOL(AbstractHistoryData *data, int period)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new MA(new TypeCastOperation<long, double>(new VOL(data)), period)));
}

PlotableCurve *PlotableCurveFactory::createBIAS(AbstractHistoryData *data, int period)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new BIAS(new CLOSE(data), period)));
}

PlotableCurve *PlotableCurveFactory::createWMS(AbstractHistoryData *data, int period)
{
    HIGH high(data);
    LOW low(data);
    CLOSE close(data);

    return new PlotableCurve(new BufferedOperationWrapper<double>(100.0 * (HHV<double>(high, period) - close) /
                                                                          (HHV<double>(high, period) - LLV<double>(low, period))));
}

PlotableCurve *PlotableCurveFactory::createLLV(AbstractHistoryData *data, int period)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new LLV<double>(new LOW(data), period)));
}

PlotableCurve *PlotableCurveFactory::createHHV(AbstractHistoryData *data, int period)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new EMA(new HIGH(data), period)));
}

PlotableCurve *PlotableCurveFactory::createATR(AbstractHistoryData *data, int period)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new ATR(data, period)));
}

PlotableCurve *PlotableCurveFactory::createOBV(AbstractHistoryData *data)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new OBV(data)));
}

PlotableCurve *PlotableCurveFactory::createTR(AbstractHistoryData *data)
{
    return new PlotableCurve(new BufferedOperationWrapper<double>(new TR(data)));
}
