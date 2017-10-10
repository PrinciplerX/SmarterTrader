#ifndef QCHARTVIEW_P_H
#define QCHARTVIEW_P_H

#include <QChartView>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

SCHARTS_BEGIN_NAMESPACE

class QChart;
class ChartPresenter;
class QChartView;

class SCHARTS_EXPORT QChartViewPrivate
{
public:
    explicit QChartViewPrivate(QChartView *q, QChart *chart = 0);
    ~QChartViewPrivate();
    void setChart(QChart *chart);
    void resize();

protected:
    QChartView *q_ptr;

public:
    QGraphicsScene *m_scene;
    QChart *m_chart;
    QPoint m_rubberBandOrigin;
#ifndef QT_NO_RUBBERBAND
    QRubberBand *m_rubberBand;
#endif
    QChartView::RubberBands m_rubberBandFlags;
};

SCHARTS_END_NAMESPACE
#endif
