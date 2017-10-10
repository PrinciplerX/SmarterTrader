#ifndef MARKETPANEL_H
#define MARKETPANEL_H

#include <QWidget>
#include <QTabBar>
#include <QTabWidget>
#include <tradingcurveplotpanel.h>
#include <realtimecurveplot.h>
#include <yahooquotefactory.h>
#include <candlestick.h>
#include <bar.h>
#include <volume.h>
#include <tickdatafactory.h>
#include "realtimepanel.h"
#include "historypanel.h"
#include "mainframe_global.h"
#include <macd.h>
#include "historydatamanager.h"
#include "realtimedatamanager.h"

using TradingCurveWidgets::TradingCurvePlot;

class MarketPanel : public QTabWidget
{
    Q_OBJECT

public:
    MarketPanel(QWidget* parent=nullptr);
    virtual ~MarketPanel(){}

public slots:
    void setData(const QString& symbol);

protected slots:
    void loadTickData(const QVector<TickData>& data);
    void loadTradeData(const QVector<TradeData>& data);

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    RealTimePanel* realTimePanel;
    HistoryPanel* historyPanel;
};

#endif // MARKETPANEL_H
