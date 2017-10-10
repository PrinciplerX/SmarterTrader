#include "fchartdataset.h"
#include "fchartpresenter.h"
#include "fchart.h"
#include <qchart_p.h>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <./axis/valueaxis/qvalueaxis_p.h>
#include <QCategoryAxis>
#include <qabstractseries_p.h>
#include <QAbstractBarSeries>
#include <QStackedBarSeries>
#include <QPercentBarSeries>
#include <QPieSeries>
#include <chartitem_p.h>

SCHARTS_BEGIN_NAMESPACE

FChartDataSet::FChartDataSet(FChart *fchart) : ChartDataSet(fchart)
{

}

SCHARTS_END_NAMESPACE
