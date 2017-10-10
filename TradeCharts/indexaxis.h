#ifndef FDATETIMEAXIS_H
#define FDATETIMEAXIS_H

#include <QObject>
#include <QValueAxis>
#include <QAbstractAxis>
#include <QValueAxis>
#include <SChartGlobal>

#include "tradecharts_global.h"
#include "indexaxiselement.h"
#include "./axis/valueaxis/qvalueaxis_p.h"

SCHARTS_USE_NAMESPACE

class IndexAxisPrivate;

class TRADECHARTS_EXPORT IndexAxis : public QValueAxis
{
    Q_OBJECT
    Q_PROPERTY(AxisLabelsPosition labelsPosition READ labelsPosition WRITE setLabelsPosition NOTIFY labelsPositionChanged)
    Q_ENUMS(AxisLabelsPosition)

public:

    enum AxisLabelsPosition {
        AxisLabelsPositionCenter = 0x0,
        AxisLabelsPositionOnValue = 0x1
    };

    explicit IndexAxis(QObject *parent = Q_NULLPTR);

    ~IndexAxis()
    {

    }

protected:
    IndexAxis(IndexAxisPrivate &d, QObject *parent = Q_NULLPTR);

public:
    AxisType type() const;

    IndexAxis::AxisLabelsPosition labelsPosition() const;

    void setLabelsPosition(IndexAxis::AxisLabelsPosition position);

    void setIndexLabels(const QVector<QString>& labels);

    const QVector<QString>& indexLabels() const;

Q_SIGNALS:
    void categoriesChanged();
    void labelsPositionChanged(IndexAxis::AxisLabelsPosition position);

private:
    QVector<QString> mIndexLabels;

    Q_DECLARE_PRIVATE(IndexAxis)
    Q_DISABLE_COPY(IndexAxis)
};

class IndexAxisPrivate : public QValueAxisPrivate
{
    Q_OBJECT

public:
    IndexAxisPrivate(IndexAxis *q)
        : QValueAxisPrivate(q),
          m_labelsPosition(IndexAxis::AxisLabelsPositionCenter)
    {

    }

    ~IndexAxisPrivate() {}

    void initializeGraphics(QGraphicsItem* parent);

private:
    IndexAxis::AxisLabelsPosition m_labelsPosition;

private:
    Q_DECLARE_PUBLIC(IndexAxis)
};

#endif // FDATETIMEAXIS_H
