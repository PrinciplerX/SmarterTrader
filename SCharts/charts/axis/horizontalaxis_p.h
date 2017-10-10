#ifndef HORIZONTALAXIS_P_H_
#define HORIZONTALAXIS_P_H_

#include <CartesianChartAxis>

SCHARTS_BEGIN_NAMESPACE

class SCHARTS_EXPORT HorizontalAxis : public CartesianChartAxis
{
public:
    HorizontalAxis(QAbstractAxis *axis, QGraphicsItem *item = 0, bool intervalAxis = false);
    ~HorizontalAxis();
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
protected:
    void updateGeometry();

};

SCHARTS_END_NAMESPACE

#endif
