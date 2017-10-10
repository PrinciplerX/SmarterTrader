#ifndef FCHARTDATASET_H
#define FCHARTDATASET_H

#include <QAbstractSeries>
#include <AbstractDomain>
#include <QAbstractAxis>
#include <QtCore/QVector>
#include <ChartDataSet>

#include "tradecharts_global.h"

SCHARTS_BEGIN_NAMESPACE

class FChart;

class TRADECHARTS_EXPORT FChartDataSet : public ChartDataSet
{
    Q_OBJECT
public:
    FChartDataSet(FChart* fchart);

private:

};

SCHARTS_END_NAMESPACE

#endif // FCHARTDATASET_H
