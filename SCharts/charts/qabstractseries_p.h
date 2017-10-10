#ifndef QABSTRACTSERIES_P_H
#define QABSTRACTSERIES_P_H

#include "qabstractseries.h"
#include "qchart.h"
#include <AbstractDomain>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
QT_END_NAMESPACE

SCHARTS_BEGIN_NAMESPACE

class ChartPresenter;
class ChartElement;
class LegendMarker;
class QLegend;
class ChartDataSet;
class QAbstractAxis;
class QLegendMarker;
class ChartTheme;
class ChartAnimation;
class ChartItem;
class BoxPlotChartItem;

class SCHARTS_EXPORT QAbstractSeriesPrivate : public QObject
{
    Q_OBJECT
public:
    QAbstractSeriesPrivate(QAbstractSeries *q);
    ~QAbstractSeriesPrivate();

    /*! \brief 根据序列数据初始化domain数值和区域范围。*/
    virtual void initializeDomain() = 0;
    /*! \brief 根据序列数据初始化已关联坐标轴的内容（通常需要遍历已关联坐标轴，根据情况依次设置）。*/
    virtual void initializeAxes() = 0;
    virtual void initializeTheme(int index, ChartTheme* theme, bool forced = false) = 0;
    virtual void initializeGraphics(QGraphicsItem* parent) = 0;
    virtual void initializeAnimations(QChart::AnimationOptions options, int duration,
                                      QEasingCurve &curve) = 0;

    virtual QList<QLegendMarker*> createLegendMarkers(QLegend* legend) = 0;

    virtual QAbstractAxis::AxisType defaultAxisType(Qt::Orientation) const = 0;
    virtual QAbstractAxis* createDefaultAxis(Qt::Orientation) const = 0;

    ChartItem* chartItem() { return m_item.data(); }

    virtual void setDomain(AbstractDomain* domain);
    AbstractDomain* domain() { return m_domain.data(); }

    virtual void setPresenter(ChartPresenter *presenter);
    ChartPresenter *presenter() const;

    QChart* chart() { return m_chart; }
    bool reverseXAxis();
    bool reverseYAxis();

    void setBlockOpenGL(bool enable);

Q_SIGNALS:
    void countChanged();

protected:
    QAbstractSeries *q_ptr;
    QChart *m_chart;
    QScopedPointer<ChartItem> m_item;
    QList<QAbstractAxis*> m_axes;

private:
    QScopedPointer<AbstractDomain> m_domain;
    QString m_name;
    bool m_visible;
    qreal m_opacity;
    ChartPresenter *m_presenter;
    bool m_useOpenGL;
    bool m_blockOpenGL;

    friend class QAbstractSeries;
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class QLegendPrivate;
    friend class BoxPlotChartItem;
};

SCHARTS_END_NAMESPACE

#endif
