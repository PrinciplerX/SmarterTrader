#include "plotableitem.h"

PlotableItem::PlotableItem()
{
    setTheme(HighContrastWhite);
}

PlotableItem::~PlotableItem(){}

double PlotableItem::regionalMaxValue(const QVector<TradeData> &data, int minIndex, int maxIndex)
{
    double result = data[minIndex].high;
    for(int i = minIndex + 1; i<maxIndex; i++){
        result = qMax(result, data[i].high);
    }
    return result;
}

double PlotableItem::regionalMinValue(const QVector<TradeData> &data, int minIndex, int maxIndex)
{
    double result = data[minIndex].low;
    for(int i = minIndex + 1; i<maxIndex; i++){
        result = qMin(result, data[i].low);
    }
    return result;
}
