#ifndef CARTESIANCHARTAXIS_H
#define CARTESIANCHARTAXIS_H

#include <SChartGlobal>
#include <ChartAxisElement>

SCHARTS_BEGIN_NAMESPACE

class QAbstractAxis;

class SCHARTS_EXPORT CartesianChartAxis : public ChartAxisElement
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsLayoutItem)
public:

    CartesianChartAxis(QAbstractAxis *axis, QGraphicsItem *item = 0, bool intervalAxis = false);
    ~CartesianChartAxis();

    void setGeometry(const QRectF &axis, const QRectF &grid);
    QRectF gridGeometry() const { return m_gridRect; }
    bool isEmpty();

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

protected:
    void setGeometry(const QRectF &size) { Q_UNUSED(size);}
    virtual void updateGeometry() = 0;
    void updateLayout(QVector<qreal> &layout);

public Q_SLOTS:
    virtual void handleArrowPenChanged(const QPen &pen);
    virtual void handleGridPenChanged(const QPen &pen);
    virtual void handleShadesBrushChanged(const QBrush &brush);
    virtual void handleShadesPenChanged(const QPen &pen);
    virtual void handleMinorArrowPenChanged(const QPen &pen);
    virtual void handleMinorGridPenChanged(const QPen &pen);
    virtual void handleGridLineColorChanged(const QColor &color);
    virtual void handleMinorGridLineColorChanged(const QColor &color);

private:
    void createItems(int count);
    void deleteItems(int count);
    void updateMinorTickItems();

private:
    QRectF m_gridRect;

    friend class AxisAnimation;
    friend class LineArrowItem;
};

SCHARTS_END_NAMESPACE

#endif /* CARTESIANCHARTAXIS_H */
