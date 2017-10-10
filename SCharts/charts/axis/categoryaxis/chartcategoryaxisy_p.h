#ifndef CHARTCATEGORYAXISY_H
#define CHARTCATEGORYAXISY_H

#include <verticalaxis_p.h>

SCHARTS_BEGIN_NAMESPACE

class QCategoryAxis;

class ChartCategoryAxisY : public VerticalAxis
{
    Q_OBJECT
public:
    ChartCategoryAxisY(QCategoryAxis *axis, QGraphicsItem* item = 0);
    ~ChartCategoryAxisY();

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

public Q_SLOTS:
    void handleCategoriesChanged();

protected:
    QVector<qreal> calculateLayout() const;
    void updateGeometry();

private:
    QCategoryAxis *m_axis;
};

SCHARTS_END_NAMESPACE

#endif /* CHARTCATEGORYAXISY_H */
