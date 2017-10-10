#ifndef CHARTVALUEAXISY_H
#define CHARTVALUEAXISY_H

#include <verticalaxis_p.h>

SCHARTS_BEGIN_NAMESPACE

class QValueAxis;

class ChartValueAxisY : public VerticalAxis
{
    Q_OBJECT
public:
    ChartValueAxisY(QValueAxis *axis, QGraphicsItem *item = 0);
    ~ChartValueAxisY();

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
protected:
    QVector<qreal> calculateLayout() const;
    void updateGeometry();
private Q_SLOTS:
    void handleTickCountChanged(int tick);
    void handleMinorTickCountChanged(int tick);
    void handleLabelFormatChanged(const QString &format);

private:
    QValueAxis *m_axis;
};

SCHARTS_END_NAMESPACE

#endif /* CHARTVALUEAXISY_H */
