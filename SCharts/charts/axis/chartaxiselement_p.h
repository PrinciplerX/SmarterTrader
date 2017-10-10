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

    // ���� QGraphicsLayoutItem �е�ͬ�� setGeometry() ������
    using QGraphicsLayoutItem::setGeometry;
public:
    ChartAxisElement(QAbstractAxis *axis, QGraphicsItem *item, bool intervalAxis = false);
    ~ChartAxisElement();

    virtual QRectF gridGeometry() const = 0;

    /*!
     * \brief �������������ʾ����
     * \param axis ��������ʾ����
     * \param grid ͼ��Ļ�ͼ����plot area����
     */
    virtual void setGeometry(const QRectF &axis, const QRectF &grid) = 0;
    virtual bool isEmpty() = 0;

    void setAnimation(AxisAnimation *animation) { m_animation = animation; }
    AxisAnimation *animation() const { return m_animation; }

    QAbstractAxis *axis() const { return m_axis; }
    void setLayout(QVector<qreal> &layout) { m_layout = layout; }
    QVector<qreal> &layout() { return m_layout; } // �ɸ��ĵ�����

    /*!
     * \todo ����label paddingֵ���̶�Ϊ4.0����С��ֵ�����¿̶Ȳ��ֳ��ȱ�ѹ��������Ϊ0.0ʱ�̶���ȫ���ɼ�������û��
     * ʲô�õĽ��������
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

    // �ñ�ʶ����������������ʾ���䣬��ǩ��labels����ʾ�ڿ̶ȣ�ticks��֮�䡣
    bool intervalAxis() const { return m_intervalAxis; }

    QStringList createValueLabels(qreal max, qreal min, int ticks, const QString &format) const;
    QStringList createLogValueLabels(qreal min, qreal max, qreal base, int ticks,
                                     const QString &format) const;
    QStringList createDateTimeLabels(qreal max, qreal min, int ticks, const QString &format) const;

    // ������ QGraphicsLayoutItem
    QRectF boundingRect() const
    {
        return QRectF();
    }

    // ������ QGraphicsLayoutItem
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
    {
    }

protected:
    /*! \brief ����̶�λ�á�ֱ��������ͼ�������Կ̶�λ���в�ͬ�Ķ��塣*/
    virtual QVector<qreal> calculateLayout() const = 0;

    /*!
     * \brief �ڿ̶���Ŀ��λ�÷����仯ʱ���²��֡�
     * \param layout �̶�λ�á�ֱ��������ͼ�������Դ��в�ͬ�Ķ��塣
     */
    virtual void updateLayout(QVector<qreal> &layout) = 0;

    /*! \return ������Ԫ�ء�*/
    QList<QGraphicsItem *> gridItems() { return m_grid->childItems(); }
    /*! \return �μ�������Ԫ�ء�*/
    QList<QGraphicsItem *> minorGridItems() { return m_minorGrid->childItems(); }
    /*! \return �̶ȱ�ǩԪ�ء�*/
    QList<QGraphicsItem *> labelItems() { return m_labels->childItems(); }
    /*! \return ��Ӱ�ڱ�Ԫ�ء�*/
    QList<QGraphicsItem *> shadeItems() { return m_shades->childItems(); }
    /*! \return �̶ȼ�ͷԪ�أ���ҪΪLineArrorItem����*/
    QList<QGraphicsItem *> arrowItems() { return m_arrow->childItems(); }
    /*! \return �μ��̶ȼ�ͷԪ�أ���ҪΪQGraphicsLineItem����*/
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
