#ifndef TRADEDATAPANEL_H
#define TRADEDATAPANEL_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include "datawidget.h"
#include "tickdatawidget.h"

class TradeDataPanel : public QWidget
{
    Q_OBJECT
public:
    explicit TradeDataPanel(QWidget *parent = 0);

protected slots:
    void onNavigationListActivated(int index);

private:
    QListWidget* mNavigationList;
    QStackedWidget* mContentWidget;

    DataWidget* dataWidget;
    TickDataWidget* tickDataWidget;
};

#endif // TRADEDATAPANEL_H
