#ifndef CANDLESTICKSET_H
#define CANDLESTICKSET_H

#include <QObject>
#include <QPen>
#include <QBrush>
#include <QFont>

#include "tradecharts_global.h"

class CSPlotSeriesPrivate;
class CandleStickSetPrivate;

class TRADECHARTS_EXPORT CandleStickSet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)

public:
    enum ValuePositions {
        LowerExtreme,
        LowerQuartile,
        UpperQuartile,
        UpperExtreme
    };

public:
    explicit CandleStickSet(const QString label = QString(), QObject *parent = Q_NULLPTR);
    explicit CandleStickSet(const qreal le, const qreal lq, const qreal uq, const qreal ue, const QString label = QString(), QObject *parent = Q_NULLPTR);
    virtual ~CandleStickSet();

    void append(const qreal value);
    void append(const QList<qreal> &values);

    void clear();

    void setLabel(const QString label);
    QString label() const;

    CandleStickSet &operator << (const qreal &value);

    void setValue(const int index, const qreal value);
    qreal at(const int index) const;
    qreal operator [](const int index) const;
    int count() const;

    void setPen(const QPen &pen);
    QPen pen() const;

    void setBrush(const QBrush &brush);
    QBrush brush() const;

    //!\todo 找到更好的解决办法
    QScopedPointer<CandleStickSetPrivate>& dPtr() { return d_ptr; }

Q_SIGNALS:
    void clicked();
    void hovered(bool status);
    void pressed();
    void released();
    void doubleClicked();
    void penChanged();
    void brushChanged();

    void valuesChanged();
    void valueChanged(int index);
    void cleared();

private:
    QScopedPointer<CandleStickSetPrivate> d_ptr;
    Q_DISABLE_COPY(CandleStickSet)
    friend class BarLegendMarker;
    friend class BarChartItem;
    friend class BoxPlotChartItem;
    friend class QBoxPlotSeriesPrivate;
};

class CandleStickSetPrivate : public QObject
{
    Q_OBJECT

public:
    CandleStickSetPrivate(const QString label, CandleStickSet *parent);
    ~CandleStickSetPrivate();

    bool append(qreal value);
    bool append(QList<qreal> values);

    int remove(const int index, const int count);
    void clear();

    void setValue(const int index, const qreal value);

    qreal value(const int index);

    //!\todo 找到更好的解决办法
    void setSeries(CSPlotSeriesPrivate* series) { m_series = series; }

Q_SIGNALS:
    void restructuredBox();
    void updatedBox();
    void updatedLayout();

    //!\todo 找到更好的解决办法
//private:
public:
    const CandleStickSet *q_ptr;
    QString m_label;
    const int m_valuesCount;
    qreal *m_values;
    int m_appendCount;
    QPen m_pen;
    QBrush m_brush;
    QBrush m_labelBrush;
    QFont m_labelFont;
    CSPlotSeriesPrivate *m_series;

    friend class CandleStickSet;
    friend class CandleStickSeriesPrivate;
};

#endif // CANDLESTICKSET_H
