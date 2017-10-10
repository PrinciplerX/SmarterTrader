#include <CartesianChartAxis>
#include <QAbstractAxis>
#include <qabstractaxis_p.h>
#include <ChartPresenter>
#include <AbstractChartLayout>
#include <AbstractDomain>
#include <linearrowitem_p.h>
#include <QValueAxis>
#include <QLogValueAxis>
#include <QtWidgets/QGraphicsLayout>
#include <QtGui/QTextDocument>

SCHARTS_BEGIN_NAMESPACE

CartesianChartAxis::CartesianChartAxis(QAbstractAxis *axis, QGraphicsItem *item , bool intervalAxis)
    : ChartAxisElement(axis, item, intervalAxis)
{
    Q_ASSERT(item);
}


CartesianChartAxis::~CartesianChartAxis()
{
}

/*! \brief ����ָ����Ŀ��tick�������ɴ����ӵ�������Ԫ�أ���̶�ָ�루arrow���������ߣ��̶ȱ�ǩ����Ӱ��shades����*/
void CartesianChartAxis::createItems(int count)
{
    // ������Ҫһ��arrow���̶ȴ���ָ�룩��
    //! \todo Ϊʲô��
    if (arrowItems().size() == 0) {
        QGraphicsLineItem *arrow = new LineArrowItem(this, this);
        arrow->setPen(axis()->linePen());
        arrowGroup()->addToGroup(arrow);
    }

    // �����ڱΣ�shades��
    if (intervalAxis() && gridItems().size() == 0) {
        for (int i = 0 ; i < 2 ; i  ++){
            QGraphicsLineItem *item = new QGraphicsLineItem(this);
            item->setPen(axis()->gridLinePen());
            gridGroup()->addToGroup(item);
            QGraphicsRectItem *shades = new QGraphicsRectItem(this);
            shades->setPen(axis()->shadesPen());
            shades->setBrush(axis()->shadesBrush());
            shadeGroup()->addToGroup(shades);
        }
    }

    // ���±���
    QGraphicsTextItem *title = titleItem();
    title->setFont(axis()->titleFont());
    title->setDefaultTextColor(axis()->titleBrush().color());
    title->setHtml(axis()->titleText());

    for (int i = 0; i < count; ++i) {
        QGraphicsLineItem *arrow = new QGraphicsLineItem(this);
        QGraphicsLineItem *grid = new QGraphicsLineItem(this);
        QGraphicsTextItem *label = new QGraphicsTextItem(this);

        // ʹ�����������ó�ʼ��
        label->document()->setDocumentMargin(ChartPresenter::textMargin());
        arrow->setPen(axis()->linePen());
        grid->setPen(axis()->gridLinePen());
        label->setFont(axis()->labelsFont());
        label->setDefaultTextColor(axis()->labelsBrush().color());
        label->setRotation(axis()->labelsAngle());

        // �����Ӧ�ļ��ϣ�group���С�
        arrowGroup()->addToGroup(arrow);
        gridGroup()->addToGroup(grid);
        labelGroup()->addToGroup(label);

        // ÿ�������������߾�����һ����Ӱ����shade��
        if (gridItems().size() == 1 || (((gridItems().size() + 1) % 2) && gridItems().size() > 0)) {
            QGraphicsRectItem *shades = new QGraphicsRectItem(this);
            shades->setPen(axis()->shadesPen());
            shades->setBrush(axis()->shadesBrush());
            shadeGroup()->addToGroup(shades);
        }
    }
}

void CartesianChartAxis::updateMinorTickItems()
{
    QValueAxis *valueAxis = qobject_cast<QValueAxis *>(this->axis());
    if (valueAxis) {
        int currentCount = minorArrowItems().size();
        int expectedCount = valueAxis->minorTickCount() * (valueAxis->tickCount() - 1);
        int diff = expectedCount - currentCount;
        if (diff > 0) {
            for (int i = 0; i < diff; i++) {
                QGraphicsLineItem *minorArrow = new QGraphicsLineItem(this);
                QGraphicsLineItem *minorGrid = new QGraphicsLineItem(this);
                minorArrow->setPen(valueAxis->linePen());
                minorGrid->setPen(valueAxis->minorGridLinePen());
                minorArrowGroup()->addToGroup(minorArrow);
                minorGridGroup()->addToGroup(minorGrid);
            }
        } else {
            QList<QGraphicsItem *> minorGridLines = minorGridItems();
            QList<QGraphicsItem *> minorArrows = minorArrowItems();
            for (int i = 0; i > diff; i--) {
                if (!minorGridLines.isEmpty())
                    delete(minorGridLines.takeLast());
                if (!minorArrows.isEmpty())
                    delete(minorArrows.takeLast());
            }
        }
    }
}

/*! \brief ����ָ����Ŀ��tick��ɾ���ɴ˼��ٵ�������Ԫ�أ���̶�ָ�루arrow���������ߣ��̶ȱ�ǩ����Ӱ��shades����*/
void CartesianChartAxis::deleteItems(int count)
{
    QList<QGraphicsItem *> lines = gridItems();
    QList<QGraphicsItem *> labels = labelItems();
    QList<QGraphicsItem *> shades = shadeItems();
    QList<QGraphicsItem *> axis = arrowItems();

    for (int i = 0; i < count; ++i) {
        // ÿɾ������griditem��ɾ��һ��shade
        if (lines.size() == 1 || (((lines.size() + 1) % 2) && lines.size() > 0))
            delete(shades.takeLast());
        delete(lines.takeLast());
        delete(labels.takeLast());
        delete(axis.takeLast());
    }
}

void CartesianChartAxis::updateLayout(QVector<qreal> &layout)
{
    int diff = ChartAxisElement::layout().size() - layout.size();

    if (diff > 0)
        deleteItems(diff);
    else if (diff < 0)
        createItems(-diff);

    updateMinorTickItems();

    // �����������ǿգ���Ӧ���ݵ�ǰ�Ķ���״̬���֡�
    if (animation()) {
        switch (presenter()->state()) {
        case ChartPresenter::ZoomInState:
            animation()->setAnimationType(AxisAnimation::ZoomInAnimation);
            animation()->setAnimationPoint(presenter()->statePoint());
            break;
        case ChartPresenter::ZoomOutState:
            animation()->setAnimationType(AxisAnimation::ZoomOutAnimation);
            animation()->setAnimationPoint(presenter()->statePoint());
            break;
        case ChartPresenter::ScrollUpState:
        case ChartPresenter::ScrollLeftState:
            animation()->setAnimationType(AxisAnimation::MoveBackwordAnimation);
            break;
        case ChartPresenter::ScrollDownState:
        case ChartPresenter::ScrollRightState:
            animation()->setAnimationType(AxisAnimation::MoveForwardAnimation);
            break;
        case ChartPresenter::ShowState:
            animation()->setAnimationType(AxisAnimation::DefaultAnimation);
            break;
        }
        animation()->setValues(ChartAxisElement::layout(), layout);
        presenter()->startAnimation(animation());
    } else {
        setLayout(layout);
        updateGeometry();
    }
}

bool CartesianChartAxis::isEmpty()
{
    return axisGeometry().isEmpty()
           || gridGeometry().isEmpty()
           || qFuzzyCompare(min(), max());
}

void CartesianChartAxis::setGeometry(const QRectF &axis, const QRectF &grid)
{
    m_gridRect = grid;
    setAxisGeometry(axis);

    if (isEmpty()) {
        prepareGeometryChange();
        return;
    }

    QVector<qreal> layout = calculateLayout();
    updateLayout(layout);
}

QSizeF CartesianChartAxis::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);
    return QSizeF();
}

void CartesianChartAxis::handleArrowPenChanged(const QPen &pen)
{
    foreach (QGraphicsItem *item, arrowItems())
        static_cast<QGraphicsLineItem *>(item)->setPen(pen);
}

void CartesianChartAxis::handleGridPenChanged(const QPen &pen)
{
    foreach (QGraphicsItem *item, gridItems())
        static_cast<QGraphicsLineItem *>(item)->setPen(pen);
}

void CartesianChartAxis::handleMinorArrowPenChanged(const QPen &pen)
{
    foreach (QGraphicsItem *item, minorArrowItems())
        static_cast<QGraphicsLineItem *>(item)->setPen(pen);
}

void CartesianChartAxis::handleMinorGridPenChanged(const QPen &pen)
{
    foreach (QGraphicsItem *item, minorGridItems())
        static_cast<QGraphicsLineItem *>(item)->setPen(pen);
}

void CartesianChartAxis::handleGridLineColorChanged(const QColor &color)
{
    foreach (QGraphicsItem *item, gridItems()) {
        QGraphicsLineItem *lineItem = static_cast<QGraphicsLineItem *>(item);
        QPen pen = lineItem->pen();
        pen.setColor(color);
        lineItem->setPen(pen);
    }
}

void CartesianChartAxis::handleMinorGridLineColorChanged(const QColor &color)
{
    foreach (QGraphicsItem *item, minorGridItems()) {
        QGraphicsLineItem *lineItem = static_cast<QGraphicsLineItem *>(item);
        QPen pen = lineItem->pen();
        pen.setColor(color);
        lineItem->setPen(pen);
    }
}

void CartesianChartAxis::handleShadesBrushChanged(const QBrush &brush)
{
    foreach (QGraphicsItem *item, shadeItems())
        static_cast<QGraphicsRectItem *>(item)->setBrush(brush);
}

void CartesianChartAxis::handleShadesPenChanged(const QPen &pen)
{
    foreach (QGraphicsItem *item, shadeItems())
        static_cast<QGraphicsRectItem *>(item)->setPen(pen);
}

#include "moc_cartesianchartaxis_p.cpp"

SCHARTS_END_NAMESPACE
