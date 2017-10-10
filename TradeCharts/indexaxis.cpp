#include "indexaxis.h"
#include <AbstractDomain>

IndexAxis::IndexAxis(QObject *parent):
    QValueAxis(*new IndexAxisPrivate(this), parent)
{
}

IndexAxis::IndexAxis(IndexAxisPrivate &d, QObject *parent):
    QValueAxis(d, parent)
{

}

QAbstractAxis::AxisType IndexAxis::type() const
{
    return QAbstractAxis::AxisTypeValue;
}

IndexAxis::AxisLabelsPosition IndexAxis::labelsPosition() const
{
    Q_D(const IndexAxis);
    return d->m_labelsPosition;
}

void IndexAxis::setLabelsPosition(IndexAxis::AxisLabelsPosition position)
{
    Q_D(IndexAxis);
    if (d->m_labelsPosition != position) {
        d->m_labelsPosition = position;
        emit labelsPositionChanged(position);
    }
}

void IndexAxis::setIndexLabels(const QVector<QString> &labels)
{
    mIndexLabels = labels;
}

const QVector<QString> &IndexAxis::indexLabels() const
{
    return mIndexLabels;
}

void IndexAxisPrivate::initializeGraphics(QGraphicsItem *parent)
{
    Q_Q(IndexAxis);
    ChartAxisElement *axis(0);
    //if(orientation() == Qt::Vertical)
    //    axis = new IndexAxisElementY(q, parent);
    //else if(orientation() == Qt::Horizontal)
    axis = new IndexAxisElementX(q, parent);
    m_item.reset(axis);
    QAbstractAxisPrivate::initializeGraphics(parent);
}
