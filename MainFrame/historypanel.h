#ifndef HISTORYPANEL_H
#define HISTORYPANEL_H

#include <QWidget>
#include <QTabBar>
#include <tradedata.h>
#include "mainframe_global.h"
#include "plotableitem.h"
#include <tradingcurveplotpanel.h>
#include <detailedpanel.h>
#include <QTableWidget>
#include <QPushButton>

using TradingCurveWidgets::TradingCurvePlotPanel;
using TradingCurveWidgets::TradingCurvePlot;

class HistoryPanel : public DetailedPanel
{
    Q_OBJECT
public:
    explicit HistoryPanel(int plotCount = 3, QWidget *parent = 0);

    /*!
     * \brief 在指定序号的TradingCurvePlot中添加一个技术指标。如果指定序号的Plot不存在，则该指标不会显示出来。
     */
    void addIndicator(int plotIndex, PlotableItem* item);

    void initButton(QPushButton* button);

    void zoomIn();
    void zoomOut();

signals:

public slots:
    void setData(AbstractHistoryData* data);
    void setPlotCount(int count);

protected slots:
    void onDataAppended();
    void onDataUpdated();
    void onDataLoaded();

    void onIndicatorBarClicked(int index);
    void onSidePanelButtonClicked();

private:
    TradingCurvePlotPanel* panel;

    QTabBar* indicatorTabBar;
    AbstractHistoryData* mData = nullptr;

    TradingCurvePlot* topPlot;
    TradingCurvePlot* midPlot;
    TradingCurvePlot* btmPlot;
};

#endif // HISTORYPANEL_H
