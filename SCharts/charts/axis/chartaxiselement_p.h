#ifndef CHARTAXISELEMENT_H
#define CHARTAXISELEMENT_H

#include <SChartGlobal>
#include <chartelement_p.h>
#include "AxisAnimation"
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsLayoutItem>
#include <QtGui/QFont>

SCHARTS_BEGIN_NAMESPACE

class ChartPresenter;
class QAbstractAxis;

class SCHARTS_EXPORT ChartAxisElement : public ChartElement, public QGraphicsLayoutItem
{
    Q_OBJECT

    // 保留 QGraphicsLayoutItem 中的同名 setGeometry() 函数。
    using QGraphicsLayoutItem::setGeometry;
public:
    ChartAxisElement(QAbstractAxis *axis, QGraphicsItem *item, bool intervalAxis = false);
    ~ChartAxisElement();

    virtual QRectF gridGeometry() const = 0;

    /*!
     * \brief 设置坐标轴的显示区域。
     * \param axis 坐标轴显示区域。
     * \param grid 图表的绘图区域（plot area）。
     */
    virtual void setGeometry(const QRectF &axis, const QRectF &grid) = 0;
    virtual bool isEmpty() = 0;

    void setAnimation(AxisAnimation *animation) { m_animation = animation; }
    AxisAnimation *animation() const { return m_animation; }

    QAbstractAxis *axis() const { return m_axis; }
    void setLayout(QVector<qreal> &layout) { m_layout = layout; }
    QVector<qreal> &layout() { return m_layout; } // 可更改的引用

    /*!
     * \todo 这里label padding值被固定为4.0，缩小该值将导致刻度部分长度被压缩（当设为0.0时刻度完全不可见）。有没有
     * 什么好的解决方法？
     */
    inline qreal labelPadding() const { return qreal(4.0); }
    inline qreal titlePadding() const { return qreal(2.0); }
    void setLabels(const QStringList &labels) { m_labelsList = labels; }
    QStringList labels() const { return m_labelsList; }

    qreal min() const;
    qreal max() const;

    QRectF axisGeometry() const { return m_axisRect; }
    void setAxisGeometry(const QRectF &axisGeometry) { m_axisRect = axisGeometry; }

    void axisSelected();

    // 该标识表明坐标轴用来显示区间，标签（labels）显示在刻度（ticks）之间。
    bool intervalAxis() const { return m_intervalAxis; }

    QStringList createValueLabels(qreal max, qreal min, int ticks, const QString &format) const;
    QStringList createLogValueLabels(qreal min, qreal max, qreal base, int ticks,
                                     const QString &format) const;
    QStringList createDateTimeLabels(qreal max, qreal min, int ticks, const QString &format) const;

    // 重载自 QGraphicsLayoutItem
    QRectF boundingRect() const
    {
        return QRectF();
    }

    // 重载自 QGraphicsLayoutItem
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
    {
    }

protected:
    /*! \brief 计算刻度位置。直角坐标轴和极坐标轴对刻度位置有不同的定义。*/
    virtual QVector<qreal> calculateLayout() const = 0;

    /*!
     * \brief 在刻度数目或位置发生变化时更新布局。
     * \param layout 刻度位置。直角坐标轴和极坐标轴对此有不同的定义。
     */
    virtual void updateLayout(QVector<qreal> &layout) = 0;

    /*! \return 网格线元素。*/
    QList<QGraphicsItem *> gridItems() { return m_grid->childItems(); }
    /*! \return 次级网格线元素。*/
    QList<QGraphicsItem *> minorGridItems() { return m_minorGrid->childItems(); }
    /*! \return 刻度标签元素。*/
    QList<QGraphicsItem *> labelItems() { return m_labels->childItems(); }
    /*! \return 阴影遮蔽元素。*/
    QList<QGraphicsItem *> shadeItems() { return m_shades->childItems(); }
    /*! \return 刻度箭头元素（主要为LineArrorItem）。*/
    QList<QGraphicsItem *> arrowItems() { return m_arrow->childItems(); }
    /*! \return 次级刻度箭头元素（主要为QGraphicsLineItem）。*/
    QList<QGraphicsItem *> minorArrowItems() { return m_minorArrow->childItems(); }

    QGraphicsTextItem *titleItem() const { return m_title.data(); }
    QGraphicsItemGroup *gridGroup() { return m_grid.data(); }
    QGraphicsItemGroup *minorGridGroup() { return m_minorGrid.data(); }
    QGraphicsItemGroup *labelGroup() { return m_labels.data(); }
    QGraphicsItemGroup *shadeGroup() { return m_shades.data(); }
    QGraphicsItemGroup *arrowGroup() { return m_arrow.data(); }
    QGraphicsItemGroup *minorArrowGroup() { return m_minorArrow.data(); }

public Q_SLOTS:
    void handleVisibleChanged(bool visible);
    void handleArrowVisibleChanged(bool visible);
    void handleGridVisibleChanged(bool visible);
    void handleLabelsVisibleChanged(bool visible);
    void handleShadesVisibleChanged(bool visible);
    void handleLabelsAngleChanged(int angle);
    virtual void handleShadesBrushChanged(const QBrush &brush) = 0;
    virtual void handleShadesPenChanged(const QPen &pen) = 0;
    virtual void handleArrowPenChanged(const QPen &pen) = 0;
    virtual void handleGridPenChanged(const QPen &pen) = 0;
    virtual void handleMinorArrowPenChanged(const QPen &pen) = 0;
    virtual void handleMinorGridPenChanged(const QPen &pen) = 0;
    virtual void handleMinorGridLineColorChanged(const QColor &color) = 0;
    virtual void handleGridLineColorChanged(const QColor &color) = 0;
    void handleLabelsBrushChanged(const QBrush &brush);
    void handleLabelsFontChanged(const QFont &font);
    void handleTitleBrushChanged(const QBrush &brush);
    void handleTitleFontChanged(const QFont &font);
    void handleTitleTextChanged(const QString &title);
    void handleTitleVisibleChanged(bool visible);
    void handleRangeChanged(qreal min, qreal max);
    void handleReverseChanged(bool reverse);
    void handleMinorArrowVisibleChanged(bool visible);
    void handleMinorGridVisibleChanged(bool visible);
    void handleLabelsPositionChanged();

Q_SIGNALS:
    void clicked();

private:
    void connectSlots();
    QString formatLabel(const QString &formatSpec, const QByteArray &array,
                        qreal value, int precision, const QString &preStr,
                        const QString &postStr) const;

    QAbstractAxis *m_axis;
    AxisAnimation *m_animation;
    QVector<qreal> m_layout;
    QStringList m_labelsList;
    QRectF m_axisRect;
    QScopedPointer<QGraphicsItemGroup> m_grid;
    QScopedPointer<QGraphicsItemGroup> m_arrow;
    QScopedPointer<QGraphicsItemGroup> m_minorGrid;
    QScopedPointer<QGraphicsItemGroup> m_minorArrow;
    QScopedPointer<QGraphicsItemGroup> m_shades;
    QScopedPointer<QGraphicsItemGroup> m_labels;
    QScopedPointer<QGraphicsTextItem> m_title;
    bool m_intervalAxis;
};

SCHARTS_END_NAMESPACE

#endif /* CHARTAXISELEMENT_H */
