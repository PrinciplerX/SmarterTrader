#ifndef TRADECHARTVIEW_H
#define TRADECHARTVIEW_H

#include <QChartView>
#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

class TRADECHARTS_EXPORT TradeChartView : public QChartView
{
public:
    explicit TradeChartView(QWidget *parent = Q_NULLPTR);
    explicit TradeChartView(QChart *chart, QWidget *parent = Q_NULLPTR);

    void resize();

    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    Q_DISABLE_COPY(TradeChartView)
};

#endif // TRADECHARTVIEW_H
