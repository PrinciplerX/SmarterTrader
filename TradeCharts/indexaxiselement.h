#ifndef INDEXAXISELEMENT_H
#define INDEXAXISELEMENT_H

#include "tradecharts_global.h"
#include <HorizontalAxis>
#include <VerticalAxis>

SCHARTS_USE_NAMESPACE

class IndexAxis;

class TRADECHARTS_EXPORT IndexAxisElementX : public HorizontalAxis
{
    Q_OBJECT
public:
    IndexAxisElementX(IndexAxis* axis, QGraphicsItem* item = 0);

    ~IndexAxisElementX() {}

    int getScaleLevel() const
    {
        return 6;
    }

    void setTickLabels(QMap<int, QString>& tickMap) { mTickMap = tickMap; }

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

protected:
    QVector<qreal> calculateLayout() const;
    void updateGeometry();

private:
    IndexAxis* mAxis;
    mutable QMap<int, QString> mTickMap;
};

#endif // INDEXAXISELEMENT_H
