#ifndef QABSTRACTAXIS_P_H
#define QABSTRACTAXIS_P_H

#include "qabstractaxis.h"
#include "chartaxiselement_p.h"
#include <QChart>
#include <QtCore/QDebug>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
QT_END_NAMESPACE

SCHARTS_BEGIN_NAMESPACE

class ChartPresenter;
class AbstractDomain;
class QChart;
class QAbstractSeries;
class ChartTheme;
class ChartElement;

class SCHARTS_EXPORT QAbstractAxisPrivate : public QObject
{
    Q_OBJECT
public:
    QAbstractAxisPrivate(QAbstractAxis *q);
    ~QAbstractAxisPrivate();

public:
    Qt::Alignment alignment() const { return m_alignment; }
    Qt::Orientation orientation() const { return m_orientation; }
    void setAlignment( Qt::Alignment alignment);

    virtual void initializeDomain(AbstractDomain *domain) = 0;
    virtual void initializeGraphics(QGraphicsItem *parent) = 0;
    virtual void initializeTheme(ChartTheme* theme, bool forced = false);
    virtual void initializeAnimations(QChart::AnimationOptions options, int duration,
                                      QEasingCurve &curve);

    // 用于从坐标轴基类中操作范围的接口。
    virtual void setMin(const QVariant &min) = 0;
    virtual void setMax(const QVariant &max) = 0;
    virtual void setRange(const QVariant &min, const QVariant &max) = 0;

    // 用于从domain对象中操作范围的接口。
    virtual void setRange(qreal min, qreal max) = 0;
    virtual qreal min() = 0;
    virtual qreal max() = 0;

    ChartAxisElement *axisItem() { return m_item.data(); }

public Q_SLOTS:
    void handleRangeChanged(qreal min, qreal max);

Q_SIGNALS:
    void rangeChanged(qreal min, qreal max);

protected:
    QAbstractAxis *q_ptr;
    QChart *m_chart;
    QScopedPointer<ChartAxisElement> m_item;

private:
    QList<QAbstractSeries*> m_series;

    Qt::Alignment m_alignment;
    Qt::Orientation m_orientation;

    bool m_visible;

    bool m_arrowVisible;
    QPen m_axisPen;
    QBrush m_axisBrush;

    bool m_gridLineVisible;
    QPen m_gridLinePen;
    bool m_minorGridLineVisible;
    QPen m_minorGridLinePen;

    bool m_labelsVisible;
    QBrush m_labelsBrush;
    QFont m_labelsFont;
    int m_labelsAngle;

    bool m_titleVisible;
    QBrush m_titleBrush;
    QFont m_titleFont;
    QString m_title;

    bool m_shadesVisible;
    QPen m_shadesPen;
    QBrush m_shadesBrush;
    qreal m_shadesOpacity;

    bool m_dirty;

    bool m_reverse;

    friend class QAbstractAxis;
    friend class ChartDataSet;
    friend class ChartPresenter;
};

SCHARTS_END_NAMESPACE

#endif
