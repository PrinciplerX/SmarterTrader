#ifndef QCHARTVIEW_H
#define QCHARTVIEW_H

#include "./axis/qabstractaxis.h"
#include "qabstractseries.h"
#include "qchart.h"
#include <QtWidgets/QGraphicsView>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QRubberBand;
QT_END_NAMESPACE

SCHARTS_BEGIN_NAMESPACE

class QChartViewPrivate;

class SCHARTS_EXPORT QChartView : public QGraphicsView
{
    Q_OBJECT
    Q_ENUMS(RubberBand)
public:

    enum  RubberBand {
        NoRubberBand = 0x0,
        VerticalRubberBand = 0x1,
        HorizontalRubberBand = 0x2,
        RectangleRubberBand = 0x3
    };

    Q_DECLARE_FLAGS(RubberBands, RubberBand)

    explicit QChartView(QWidget *parent = Q_NULLPTR);
    explicit QChartView(QChart *chart, QWidget *parent = Q_NULLPTR);
    ~QChartView();

    void setRubberBand(const RubberBands &rubberBands);
    RubberBands rubberBand() const;

    QChart *chart() const;
    void setChart(QChart *chart);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    QScopedPointer<QChartViewPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QChartView)
};

SCHARTS_END_NAMESPACE

#endif // QCHARTVIEW_H
