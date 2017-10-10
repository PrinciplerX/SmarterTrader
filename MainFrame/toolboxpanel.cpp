#include "toolboxpanel.h"
#include <QtWidgets>

ToolBoxPanel::ToolBoxPanel(QWidget *parent):
    QTabWidget(parent),
    testPanel(new TestPanel()),
    chartPanel(new ChartTestPanel())
{
    tabBar()->setObjectName(QString("PanelTabBar"));

    this->addTab(testPanel,tr(""));
    this->addTab(chartPanel,tr("SCharts"));
}

