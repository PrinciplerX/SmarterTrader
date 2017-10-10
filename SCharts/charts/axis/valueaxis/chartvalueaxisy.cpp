#include <chartvalueaxisy_p.h>
#include <QAbstractAxis>
#include <chartpresenter_p.h>
#include "../axis/valueaxis/qvalueaxis.h"
#include <abstractchartlayout_p.h>
#include <QtWidgets/QGraphicsLayout>
#include <QtCore/QtMath>
#include <QtCore/QDebug>

SCHARTS_BEGIN_NAMESPACE

ChartValueAxisY::ChartValueAxisY(QValueAxis *axis, QGraphicsItem *item)
    : VerticalAxis(axis, item),
      m_axis(axis)
{
    QObject::connect(m_axis, SIGNAL(tickCountChanged(int)), this, SLOT(handleTickCountChanged(int)));
    QObject::connect(m_axis, SIGNAL(minorTickCountChanged(int)),
                     this, SLOT(handleMinorTickCountChanged(int)));
    QObject::connect(m_axis, SIGNAL(labelFormatChanged(QString)), this, SLOT(handleLabelFormatChanged(QString)));
}

ChartValueAxisY::~ChartValueAxisY()
{
}

QVector<qreal> ChartValueAxisY::calculateLayout() const
{
    int tickCount = m_axis->tickCount();

    Q_ASSERT(tickCount >= 2);

    QVector<qreal> points;
    points.resize(tickCount);

    const QRectF &gridRect = gridGeometry();

    const qreal deltaY = gridRect.height() / (qreal(tickCount) - 1.0);
    for (int i = 0; i < tickCount; ++i)
        points[i] = qreal(i) * -deltaY + gridRect.bottom();

    return points;
}

void ChartValueAxisY::updateGeometry()
{
    const QVector<qreal> &layout = ChartAxisElement::layout();
    if (layout.isEmpty())
        return;
    setLabels(createValueLabels(min(),max(),layout.size(),m_axis->labelFormat()));
    VerticalAxis::updateGeometry();
}

void ChartValueAxisY::handleTickCountChanged(int tick)
{
    Q_UNUSED(tick);
    QGraphicsLayoutItem::updateGeometry();
    if (presenter()) presenter()->layout()->invalidate();
}

void ChartValueAxisY::handleMinorTickCountChanged(int tick)
{
    Q_UNUSED(tick);
    QGraphicsLayoutItem::updateGeometry();
    if (presenter())
        presenter()->layout()->invalidate();
}

void ChartValueAxisY::handleLabelFormatChanged(const QString &format)
{
    Q_UNUSED(format);
    QGraphicsLayoutItem::updateGeometry();
    if(presenter()) presenter()->layout()->invalidate();
}

QSizeF ChartValueAxisY::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint)

    QSizeF sh;
    QSizeF base = VerticalAxis::sizeHint(which, constraint);
    QStringList ticksList = createValueLabels(min(),max(),m_axis->tickCount(),m_axis->labelFormat());
    qreal width = 0;
    // Height of vertical axis sizeHint indicates the maximum distance labels can extend past
    // first and last ticks. Base height is irrelevant.
    qreal height = 0;

    switch (which) {
    case Qt::MinimumSize: {
        QRectF boundingRect = ChartPresenter::textBoundingRect(axis()->labelsFont(),
                                                               QStringLiteral("..."),
                                                               axis()->labelsAngle());
        width = boundingRect.width() + labelPadding() + base.width() + 1.0;
        height = boundingRect.height() / 2.0;
        sh = QSizeF(width, height);
        break;
    }
    case Qt::PreferredSize: {
        qreal labelWidth = 0.0;
        qreal firstHeight = -1.0;
        foreach (const QString& s, ticksList) {
            QRectF rect = ChartPresenter::textBoundingRect(axis()->labelsFont(), s, axis()->labelsAngle());
            labelWidth = qMax(rect.width(), labelWidth);
            height = rect.height();
            if (firstHeight < 0.0)
                firstHeight = height;
        }
        width = labelWidth + labelPadding() + base.width() + 2.0; //two pixels of tolerance
        height = qMax(height, firstHeight) / 2.0;
        sh = QSizeF(width, height);
        break;
    }
    default:
        break;
    }
    return sh;
}

#include "moc_chartvalueaxisy_p.cpp"

SCHARTS_END_NAMESPACE
