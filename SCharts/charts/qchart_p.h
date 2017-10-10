#ifndef QCHART_P_H
#define QCHART_P_H

#include "qchartglobal.h"
#include "qchart.h"

SCHARTS_BEGIN_NAMESPACE

class ChartThemeManager;
class ChartPresenter;
class QLegend;
class ChartDataSet;

/*!
 * \brief QChart���ڲ����ݳ�Ա����
 *
 * QChart ���󲿷��ڲ��������ɵ� ChartPresenter �� ChartDataSet �� ChartThemeManager ���С�
 */
class SCHARTS_EXPORT QChartPrivate
{
public:
    QChartPrivate(QChart *q, QChart::ChartType type);
    ~QChartPrivate();
    QChart *q_ptr;
    QLegend *m_legend;
    ChartDataSet *m_dataset;
    ChartPresenter *m_presenter;
    ChartThemeManager *m_themeManager;
    QChart::ChartType m_type;

    static QPen &defaultPen();
    static QBrush &defaultBrush();
    static QFont &defaultFont();

    void init();
    void zoomIn(qreal factor);
    void zoomOut(qreal factor);
    void zoomIn(const QRectF &rect);
    void zoomReset();
    bool isZoomed();
    void scroll(qreal dx, qreal dy);
};

SCHARTS_END_NAMESPACE
#endif
