#ifndef CANDLESTICK_H
#define CANDLESTICK_H

#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include "candlestickset.h"
#include <AbstractDomain>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "candlestickdata.h"
#include "tradecharts_global.h"

SCHARTS_USE_NAMESPACE

class TRADECHARTS_EXPORT CandleStick : public QGraphicsObject
{
    Q_OBJECT
public:
    CandleStick(CandleStickSet* set, AbstractDomain* domain, QGraphicsObject* parent):
        QGraphicsObject(parent),
        m_boxSet(set),
        m_domain(domain),
        m_mousePressed(false)
    {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::MouseButtonMask);
        setFlag(QGraphicsObject::ItemIsSelectable);
    }

    ~CandleStick() {}

    void setBrush(const QBrush &brush)
    {
        m_brush = brush;
        m_outlinePen.setColor(m_brush.color());
        update();
    }

    void setPen(const QPen &pen)
    {
        // 画笔变化需要更新边界范围。
        qreal widthDiff = pen.widthF() - m_pen.widthF();
        m_boundingRect.adjust(-widthDiff, -widthDiff, widthDiff, widthDiff);

        m_pen = pen;
        m_medianPen = pen;
        m_medianPen.setCapStyle(Qt::FlatCap);
        m_outlinePen = pen;
        m_outlinePen.setStyle(Qt::SolidLine);
        m_outlinePen.setColor(m_brush.color());

        update();
    }

    void setLayout(const CandleStickData &data)
    {
        m_data = data;

        updateGeometry(m_domain);
        update();
    }

    void setBoxOutlined(const bool outlined) { m_boxOutlined = outlined; }
    void setBoxWidth(const qreal width)
    {
        m_boxWidth = width;
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        Q_UNUSED(event)
        emit pressed(m_boxSet);
        m_mousePressed = true;
    }

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)
    {
        Q_UNUSED(event)
        emit hovered(true, m_boxSet);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
    {
        Q_UNUSED(event)
        emit hovered(true, m_boxSet);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        Q_UNUSED(event)
        emit released(m_boxSet);
        if(m_mousePressed)
            emit clicked(m_boxSet);
    }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        Q_UNUSED(event)
        emit pressed(m_boxSet);
        emit doubleClicked(m_boxSet);
    }

    QRectF boundingRect() const
    {
        return m_boundingRect;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)

        painter->save();
        painter->setBrush(m_brush);
        painter->setClipRect(parentItem()->boundingRect());
        painter->setPen(m_pen);

        painter->drawPath(m_boxPath);
        if (!m_boxOutlined)
            painter->setPen(m_outlinePen);
        painter->drawRect(m_middleBox);
        painter->restore();
    }

    void updateGeometry(AbstractDomain *domain)
    {
        m_domain = domain;

        prepareGeometryChange();

        QPainterPath path;
        m_boxPath = path;
        m_boundingRect = m_boxPath.boundingRect();

        qreal columnWidth = 1.0 / m_data.m_seriesCount;
        qreal left = ((1.0 - m_boxWidth) / 2.0) * columnWidth + columnWidth * m_data.m_seriesIndex + m_data.m_index - 0.5;
        qreal barWidth = m_boxWidth * columnWidth;

        QPointF geometryPoint = m_domain->calculateGeometryPoint(QPointF(left, m_data.m_upperExtreme), m_validData);
        if (!m_validData)
            return;
        m_geometryLeft = geometryPoint.x();
        qreal geometryUpperExtreme = geometryPoint.y();
        geometryPoint = m_domain->calculateGeometryPoint(QPointF(left + barWidth, m_data.m_upperQuartile), m_validData);
        if (!m_validData)
            return;
        m_geometryRight = geometryPoint.x();
        qreal geometryUpperQuartile = geometryPoint.y();
        geometryPoint = m_domain->calculateGeometryPoint(QPointF(left, m_data.m_lowerQuartile), m_validData);
        if (!m_validData)
            return;
        qreal geometryLowerQuartile = geometryPoint.y();
        geometryPoint = m_domain->calculateGeometryPoint(QPointF(left, m_data.m_lowerExtreme), m_validData);
        if (!m_validData)
            return;
        qreal geometryLowerExtreme = geometryPoint.y();

        // 上影线
        path.moveTo((m_geometryLeft + m_geometryRight) / 2.0, geometryUpperExtreme);
        path.lineTo((m_geometryLeft + m_geometryRight) / 2.0, geometryUpperQuartile);

        // 实心柱线
        m_middleBox.setCoords(m_geometryLeft, geometryUpperQuartile, m_geometryRight, geometryLowerQuartile);

        // 下影线
        path.moveTo((m_geometryLeft + m_geometryRight) / 2.0, geometryLowerQuartile);
        path.lineTo((m_geometryLeft + m_geometryRight) / 2.0, geometryLowerExtreme);

        path.closeSubpath();

        m_boxPath = path;
        m_boundingRect.setCoords(m_geometryLeft, geometryUpperExtreme, m_geometryRight, geometryLowerExtreme);

        qreal extra = m_pen.widthF();
        m_boundingRect.adjust(-extra, -extra, extra, extra);
    }

protected:
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
    {
        Q_UNUSED(which)
        Q_UNUSED(constraint)

        return QSizeF();
    }

    void setGeometry(const QRectF &rect)
    {
        Q_UNUSED(rect)
    }

Q_SIGNALS:
    void clicked(CandleStickSet *boxset);
    void hovered(bool status, CandleStickSet *boxset);
    void pressed(CandleStickSet *boxset);
    void released(CandleStickSet *boxset);
    void doubleClicked(CandleStickSet *boxset);

private:
    friend class CSPlotChartItem;
    friend class CSPlotAnimation;

    CandleStickSet *m_boxSet;
    AbstractDomain *m_domain;
    QPainterPath    m_boxPath;
    QRectF m_boundingRect;
    bool m_hovering;
    bool m_validData;
    QBrush  m_brush;
    QPen m_pen;
    QPen m_medianPen;
    QPen m_outlinePen;
    bool m_boxOutlined;
    qreal m_boxWidth;
    CandleStickData m_data;
    QSizeF m_domainSize;
    QRectF m_middleBox;
    qreal m_geometryLeft;
    qreal m_geometryRight;

    bool m_mousePressed;
};

Q_DECLARE_METATYPE(QVector<QRectF>)
Q_DECLARE_METATYPE(CandleStickData)
Q_DECLARE_METATYPE(qreal)

#endif // CANDLESTICK_H
