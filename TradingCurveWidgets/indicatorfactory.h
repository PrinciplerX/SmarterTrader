#ifndef INDICATORFACTORY_H
#define INDICATORFACTORY_H

#include "tradingcurvewidgets_global.h"
#include "tradedata.h"
#include "techniqueindicator.h"

class TRADINGCURVEWIDGETSSHARED_EXPORT IndicatorFactory
{
public:
    IndicatorFactory();

    static TechniqueIndicator* buildIndicator(const QString name);
    static QString indicatorName();
};

#endif // INDICATORFACTORY_H
