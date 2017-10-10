#include "barchart.h"

BarChart::BarChart()
{

}

void BarChart::append(BarSet *item)
{

}

double BarChart::regionalMaxValue(int minIndex, int maxIndex) const
{
    return 100;
}

double BarChart::regionalMinValue(int minIndex, int maxIndex) const
{
    return 0;
}

int BarChart::tracerTarget(int itemIndex, const QwtScaleMap &yMap) const
{
    return 0;
}

QString BarChart::dateString(int index)
{
    return QString("BarSertDate");
}

QString BarChart::titleLabelString(int index)
{
    return QString("BarSertDate");
}

BarSet &BarSet::operator<<(double value)
{
    valueList.append(value);
    return (*this);
}
