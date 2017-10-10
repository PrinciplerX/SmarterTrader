#ifndef REALTIMEPANEL_H
#define REALTIMEPANEL_H

#include <realtimecurveplot.h>
#include <realtimeplot.h>
#include <QWidget>
#include <tickdatafactory.h>
#include <ticktable.h>
#include <tickmodel.h>
#include <globalaverage.h>
#include <restingordertable.h>
#include "mainframe_global.h"
#include <realtimeplot.h>
#include <realtimeplotpanel.h>
#include "splitterplot.h"
#include "quotationlabel.h"
#include <QSplitter>
#include <QStackedWidget>
#include <realtimecompositeplot.h>

using TradingCurveWidgets::RealTimePlotPanel;
using TradingCurveWidgets::RealTimePlot;
using TradingCurveWidgets::RealTimeCurvePlot;
using TradingCurveWidgets::RealTimeCompositePlot;

class RealTimePanel : public QWidget
{
    Q_OBJECT
public:
    RealTimePanel(QWidget* parent = nullptr);

public slots:
    void setData(const AbstractRealTimeData *data);
    void setTickData(const AbstractTickData* tickData);
    void onDataUpdated();

protected slots:
    void onTickDataLoaded();

private:
    RestingOrderTable* restingOrderTable;
    TickTable* tickTable;
    QuotationLabel* quoteLabel;
    RealTimeCompositePlot* realTimeCompositePlot;
    const AbstractRealTimeData* mData = nullptr;
    const AbstractTickData* mTickData = nullptr;

    QStackedWidget* stackedWidget;
};

#endif // REALTIMEPANEL_H
