#include "indexaxiselement.h"
#include "indexaxis.h"
#include <ChartPresenter>
#include <QtMath>

IndexAxisElementX::IndexAxisElementX(IndexAxis *axis, QGraphicsItem *item):
    HorizontalAxis(axis, item, true),
    mAxis(axis)
{
}

QSizeF IndexAxisElementX::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(constraint)

    QSizeF sh;
    QSizeF base = HorizontalAxis::sizeHint(which, constraint);
    QList<QString> ticksList = mTickMap.values();
    qreal width = 0; // Width is irrelevant for X axes with interval labels
    qreal height = 0;

    switch (which) {
    case Qt::MinimumSize: {
        QRectF boundingRect = ChartPresenter::textBoundingRect(axis()->labelsFont(),
                                                               QStringLiteral("..."),
                                                               axis()->labelsAngle());
        height = boundingRect.height() + labelPadding() + base.height() + 1.0;
        sh = QSizeF(width, height);
        break;
    }
    case Qt::PreferredSize: {
        qreal labelHeight = 0.0;
        foreach (const QString& s, ticksList) {
            QRectF rect = ChartPresenter::textBoundingRect(axis()->labelsFont(), s, axis()->labelsAngle());
            labelHeight = qMax(rect.height(), labelHeight);
        }
        height = labelHeight + labelPadding() + base.height() + 1.0;
        sh = QSizeF(width, height);
        break;
    }
    default:
        break;
    }

    return sh;
}

QVector<qreal> IndexAxisElementX::calculateLayout() const
{
    int min = qRound(mAxis->min());
    int max = qRound(mAxis->max());
    int tickCount = max - min + 1;

    if (tickCount <= 0) return QVector<qreal>();

    QMap<int, QString> tickMap;
    const QVector<QString>& labels = mAxis->indexLabels();

    if (getScaleLevel() < 2) {
        //每天都显示当月第几天
        for (int i = min; i < max; i++) {
            tickMap.insert(i - min, labels[i].right(2));
        }
    } else if (getScaleLevel()<9) {
        QString month = labels[min].mid(5, 2);
        tickMap.insert(0, month);

        //每月第一天显示月份
        for (int i = min + 1; i < max; i++) {
            QString currentMonth = labels[i].mid(5, 2);
            if (month != currentMonth) {
                month = currentMonth;
                tickMap.insert(i - min, currentMonth);
            }
        }
    } else {
        //每年第一天显示年份
        QString year = labels[min].left(4);
        tickMap.insert(0, year);

        //每月第一天显示月份
        for (int i = min + 1; i<max; i++) {
            QString currentYear = labels[i].left(4);
            if (year != currentYear) {
                year = currentYear;
                tickMap.insert(i - min, currentYear);
            }
        }
    }

    const QRectF &gridRect = gridGeometry();
    qreal range = this->max() - this->min();
    if (range <= 0) return QVector<qreal>();

    qreal scale = gridRect.width() / range;

    QVector<qreal> tickPos(tickMap.size() + 1);
    QMap<int, QString>::const_iterator i = tickMap.cbegin();
    int counter = 0;
    while (i != tickMap.end()) {
        tickPos[counter] = i.key() * scale + gridRect.left();
        counter++;
        i++;
    }
    tickPos[tickPos.size() - 1] = (max - min + 1) * scale + gridRect.left();

    mTickMap = tickMap;

    return tickPos;
}

void IndexAxisElementX::updateGeometry()
{
    QStringList labels;
    QMap<int, QString>::const_iterator i = mTickMap.cbegin();
    while (i != mTickMap.end()){
        labels.append(i.value());
        i++;
    }

    setLabels(labels << QString());

    HorizontalAxis::updateGeometry();
}
