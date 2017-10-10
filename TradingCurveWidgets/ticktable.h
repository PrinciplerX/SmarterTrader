#ifndef TICKTABLE_H
#define TICKTABLE_H

#include "tradingcurvewidgets_global.h"
#include <tickmodel.h>
#include <QWidget>
#include <QTableView>

class TRADINGCURVEWIDGETSSHARED_EXPORT TickTable : public QWidget
{
    Q_OBJECT
public:
    explicit TickTable(QWidget *parent = nullptr);

    void setModel(TickModel* model);

signals:

public slots:

private:
    QTableView* tickTable;
};

#endif // TICKTABLE_H
