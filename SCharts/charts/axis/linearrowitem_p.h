#ifndef LINEARROWITEM_P_H
#define LINEARROWITEM_P_H

#include <chartaxiselement_p.h>
#include <qabstractaxis_p.h>
#include <QtWidgets/QGraphicsLineItem>

SCHARTS_BEGIN_NAMESPACE

/*! \brief 直角坐标上的主刻度指针。*/
class LineArrowItem: public QGraphicsLineItem
{
public:
    explicit LineArrowItem(ChartAxisElement *axis, QGraphicsItem *parent = 0)
        : QGraphicsLineItem(parent),
          m_axis(axis),
          m_axisOrientation(axis->axis()->orientation())
    {
    }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        Q_UNUSED(event)
        m_axis->axisSelected();
    }

    QRectF boundingRect() const
    {
        return shape().boundingRect();
    }

    QPainterPath shape() const
    {
        QPainterPath path = QGraphicsLineItem::shape();
        QRectF rect = path.boundingRect();
        // 在坐标轴方向上增加了长度为8的区域。
        path.addRect(rect.adjusted(0, 0, m_axisOrientation != Qt::Horizontal ? 8 : 0, m_axisOrientation != Qt::Vertical ? 8 : 0));
        return path;
    }

private:
    ChartAxisElement *m_axis;
    Qt::Orientation m_axisOrientation;
};

SCHARTS_END_NAMESPACE

#endif /* LINEARROWITEM_P_H */
