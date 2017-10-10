#ifndef TOOLBOXPANEL_H
#define TOOLBOXPANEL_H

#include <QWidget>
#include <QTabWidget>
#include "mainframe_global.h"
#include "charttestpanel.h"
#include "testpanel.h"

class ToolBoxPanel : public QTabWidget
{
    Q_OBJECT
public:
    explicit ToolBoxPanel(QWidget *parent = 0);

signals:

public slots:

private:
    TestPanel* testPanel;
    ChartTestPanel* chartPanel;
};

#endif // TOOLBOXPANEL_H
