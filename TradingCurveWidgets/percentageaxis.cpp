#include "percentageaxis.h"

namespace TradingCurveWidgets{

PercentageAxis::PercentageAxis(double baseValue, AbstractAxis::AxisType align, AbstractItemPlot *parent) :
    TitledPlotAxis(align, parent),
    scaleDraw(new PercentageScaleDraw(baseValue))
{
    this->setScaleDraw(scaleDraw);
}

void PercentageAxis::setBaseValue(double value){
    scaleDraw->setBaseValue(value);
}

}
