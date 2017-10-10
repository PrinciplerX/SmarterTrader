#include "tradechartview.h"
#include "qchartview_p.h"
#include <QValueAxis>

TradeChartView::TradeChartView(QWidget *parent):
    QChartView(parent)
{

}

TradeChartView::TradeChartView(QChart *chart, QWidget *parent):
    QChartView(chart, parent)
{

}

void TradeChartView::resize()
{
    d_ptr.data()->resize();
}

void TradeChartView::resizeEvent(QResizeEvent *event)
{
    QChartView::resizeEvent(event);
}

void TradeChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        chart()->axisX()->setRange(79.5, 120.5);
        chart()->axisY()->setRange(1500, 4850);
        break;
    case Qt::Key_Down:
        chart()->axisX()->setRange(-0.5, 150.5);
        chart()->axisY()->setRange(998.2, 6124);
        break;
    case Qt::Key_Left:{
        qreal min = ((QValueAxis*)chart()->axisX())->min() - 1;
        qreal max = ((QValueAxis*)chart()->axisX())->max() - 1;
        chart()->axisX()->setRange(min, max);
        break;
    }
    case Qt::Key_Right:{
        qreal min = ((QValueAxis*)chart()->axisX())->min() + 1;
        qreal max = ((QValueAxis*)chart()->axisX())->max() + 1;
        chart()->axisX()->setRange(min, max);
        break;
    }

    }
}
