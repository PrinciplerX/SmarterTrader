#ifndef VERTICALAXIS_P_H_
#define VERTICALAXIS_P_H_

#include <CartesianChartAxis>

SCHARTS_BEGIN_NAMESPACE

class SCHARTS_EXPORT VerticalAxis : public CartesianChartAxis
{
public:
    VerticalAxis(QAbstractAxis *axis, QGraphicsItem *item = 0, bool intervalAxis = false);
    ~VerticalAxis();
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
protected:
    void updateGeometry();

};

SCHARTS_END_NAMESPACE

#endif /* VERTICALAXIS_P_H_ */
