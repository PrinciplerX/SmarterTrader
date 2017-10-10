#include <chartvalueaxisx_p.h>
#include <QAbstractAxis>
#include <ChartPresenter>
#include <QValueAxis>
#include <AbstractChartLayout>
#include <QtWidgets/QGraphicsLayout>
#include <QtCore/QtMath>
#include <QtCore/QDebug>


SCHARTS_BEGIN_NAMESPACE

ChartValueAxisX::ChartValueAxisX(QValueAxis *axis, QGraphicsItem *item )
    : HorizontalAxis(axis, item),
      m_axis(axis)
{
    QObject::connect(m_axis, SIGNAL(tickCountChanged(int)), this, SLOT(handleTickCountChanged(int)));
    QObject::connect(m_axis, SIGNAL(minorTickCountChanged(int)),
                     this, SLOT(handleMinorTickCountChanged(int)));
    QObject::connect(m_axis, SIGNAL(labelFormatChanged(QString)), this, SLOT(handleLabelFormatChanged(QString)));
}

ChartValueAxisX::~ChartValueAxisX()
{
}

/*! \return ����ticks��ˮƽλ�á�*/
QVector<qreal> ChartValueAxisX::calculateLayout() const
{
    int tickCount = m_axis->tickCount();

    Q_ASSERT(tickCount >= 2);

    QVector<qreal> points;
    points.resize(tickCount);

    const QRectF &gridRect = gridGeometry();
    const qreal deltaX = gridRect.width() / (qreal(tickCount) - 1.0);
    for (int i = 0; i < tickCount; ++i)
        points[i] = qreal(i) * deltaX + gridRect.left();
    return points;
}

void ChartValueAxisX::updateGeometry()
{
    const QVector<qreal>& layout = ChartAxisElement::layout();
    if (layout.isEmpty())
        return;
    setLabels(createValueLabels(min(), max(), layout.size(), m_axis->labelFormat()));
    HorizontalAxis::updateGeometry();
}

void ChartValueAxisX::handleTickCountChanged(int tick)
{
    Q_UNUSED(tick);
    QGraphicsLayoutItem::updateGeometry();
    if(presenter()) presenter()->layout()->invalidate();
}

void ChartValueAxisX::handleMinorTickCountChanged(int tick)
{
    Q_UNUSED(tick);
    QGraphicsLayoutItem::updateGeometry();
    if (presenter())
        presenter()->layout()->invalidate();
}

void ChartValueAxisX::handleLabelFormatChanged(const QString &format)
{
    Q_UNUSED(format);
    QGraphicsLayoutItem::updateGeometry();
    if(presenter()) presenter()->layout()->invalidate();
}

QSizeF ChartValueAxisX::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint)

    QSizeF sh;

    QSizeF base = HorizontalAxis::sizeHint(which, constraint);
    QStringList ticksList = createValueLabels(min(),max(),m_axis->tickCount(),m_axis->labelFormat());
    // Width of horizontal axis sizeHint indicates the maximum distance labels can extend past
    // first and last ticks. Base width is irrelevant.
    qreal width = 0;
    qreal height = 0;

    switch (which) {
        case Qt::MinimumSize: {
            QRectF boundingRect = ChartPresenter::textBoundingRect(axis()->labelsFont(),
                                                                   QStringLiteral("..."),
                                                                   axis()->labelsAngle());
            width = boundingRect.width() / 2.0;
            height = boundingRect.height() + labelPadding() + base.height() + 1.0;
            sh = QSizeF(width, height);
            break;
        }
        case Qt::PreferredSize: {
            qreal labelHeight = 0.0;
            qreal firstWidth = -1.0;
            foreach (const QString& s, ticksList) {
                QRectF rect = ChartPresenter::textBoundingRect(axis()->labelsFont(), s, axis()->labelsAngle());
                labelHeight = qMax(rect.height(), labelHeight);
                width = rect.width();
                if (firstWidth < 0.0)
                    firstWidth = width;
            }
            height = labelHeight + labelPadding() + base.height() + 1.0;
            width = qMax(width, firstWidth) / 2.0;
            sh = QSizeF(width, height);
            break;
        }
        default:
            break;
    }
    return sh;
}

#include "moc_chartvalueaxisx_p.cpp"

SCHARTS_END_NAMESPACE
