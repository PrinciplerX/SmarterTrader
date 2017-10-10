#include "indicatorfactory.h"

IndicatorFactory::IndicatorFactory()
{

}

TechniqueIndicator *IndicatorFactory::buildIndicator(const QString name)
{
    return nullptr;
}

QString IndicatorFactory::indicatorName()
{
    return QString();
}
